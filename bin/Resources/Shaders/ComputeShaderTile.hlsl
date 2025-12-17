//--------------------------------------------------------------------------------------
// File: ComputeShaderTile 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderDefines.h"
#include "ShaderBuffers.hlsl"

Texture2D GAlbedo_GDepth					: register(t0);
Texture2D GNormal_RoughMetal				: register(t1);
Texture2D GPosition							: register(t2);
StructuredBuffer<DirectionalLight> gLight	: register(t3);
RWStructuredBuffer<uint2> gFramebuffer		: register(u0);

groupshared uint sMinZ;
groupshared uint sMaxZ;

// Light list for the tile
groupshared uint sTileLightIndices[MAX_LIGHTS];
groupshared uint sTileNumLights;

// List of pixels that require per-sample shading
// We encode two 16-bit x/y coordinates in one uint to save shared memory space
groupshared uint sPerSamplePixels[COMPUTE_SHADER_TILE_GROUP_SIZE];
groupshared uint sNumPerSamplePixels;

//--------------------------------------------------------------------------------------
// Shader Functions
//--------------------------------------------------------------------------------------
// Utility for writing to our flat MSAAed UAV
void WriteSample(uint2 coords, float4 value)
{
	gFramebuffer[GetFramebufferSampleAddress(coords)] = PackRGBA16(value);
}

//Obtiene la informacion de cada sampler(en nuestro caso, por ahora 1)
SurfaceData ComputeSurfaceDataFromGBufferSample(uint2 positionViewport)
{
	// Load the raw data from the GBuffer
	GBuffer_OUTPUT rawData;
	rawData.GNormal_RoughMetal = GNormal_RoughMetal.Load(int3(positionViewport.xy, 0));
	rawData.GAlbedo_GDepth		= GAlbedo_GDepth.Load(int3(positionViewport.xy, 0));
	rawData.GPosition			= GPosition.Load(int3(positionViewport.xy, 0));
	float zBuffer				= rawData.GAlbedo_GDepth.w;

	float2 gbufferDim;
	GAlbedo_GDepth.GetDimensions(gbufferDim.x, gbufferDim.y);

	// Decode into reasonable outputs
	SurfaceData data;

	data.position = rawData.GPosition.xyz;

	data.normal = DecodeSphereMap(rawData.GNormal_RoughMetal.xy);
	data.albedo = float4(rawData.GAlbedo_GDepth.xyz, 1.0f);

	data.Roughness = rawData.GNormal_RoughMetal.z;
	data.Metallic = rawData.GNormal_RoughMetal.w;

	return data;
}
////////////////////////LIGTH COMPUTE///////////////////////////////////

float3 Diffuse(float3 pAlbedo)
{
	return pAlbedo / M_PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
	//GGX ISOTROPICO
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	float denominador = NdH2 * (a2 - 1.0f) + 1.0f;
	denominador *= denominador;
	denominador *= M_PI;

	return a2 / denominador;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
	//GGX Smith Schick
	float K = a * 0.5f; //Constante de absorcion
	float GV = NdV / (NdV * (1 - K) + K); //arametro de distribucion sobre la vista, lo q percibes segun el angulo de vista
	float GL = NdL / (NdL * (1 - K) + K);//segun lo q percives de rebote

	return GV * GL;
}

float3 Fresnel_Schlick(float3 specularColor, float3 v, float3 h)
{
	return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float Specular_D(float a, float NdH) // Distribucion de normales
{
	return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) // Valor geometrico 
{
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Specular_F(float3 specularColor, float3 v, float3 h)
{
	return Fresnel_Schlick(specularColor, v, h);
}

float Specular_F_Roughness(float3 specularColor, float a, float3 h, float3 v)
{
	return (specularColor + (max(1.0f - a, specularColor) - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float3 a, float NdL, float NdV, float NdH, float VdH, float LdV)//Aporte especular llenando el compute light
{
	return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f); // 
}

float3 ComputeLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightPosition, float3 lightColor, float3 lightDir, float3 viewDir)
{
	//Calculamos los vectores del brdf
	float NdL = saturate(dot(normal, lightDir));
	float NdV = saturate(dot(normal, viewDir));
	float3 h = normalize(lightDir + viewDir);
	float NdH = saturate(dot(normal, h));
	float VdH = saturate(dot(viewDir, h));
	float LdV = saturate(dot(lightDir, viewDir));
	float a = max(0.001f, roughness * roughness);//el numero es para seguridad, ya q el resultado se usara como divisor

	float3 ColorDiffuse = Diffuse(albedoColor);
	float3 ColorSpecular = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);

	return lightColor * NdL * (ColorDiffuse * (1.0f - ColorSpecular) + ColorSpecular);
}


// Uses an in-out for accumulation to avoid returning and accumulating 0
void AccumulateBRDF(SurfaceData surface, DirectionalLight light, inout float3 lit)
{
	float3 viewDir = normalize(viewPos.xyz - surface.position);
	float3 lightDir = normalize(light.LightPosition.xyz - surface.position);
	float3 realAlbedo = surface.albedo.xyz - surface.albedo.xyz * surface.Metallic;
	float3 realSpecularColor = lerp(0.03f, surface.albedo.xyz, surface.Metallic);

	float3 LightColor = ComputeLight(realAlbedo, realSpecularColor, surface.normal, surface.Roughness, light.LightPosition.xyz, light.LightColor.xyz, lightDir, viewDir);

	//Salida Final
	float3 FinalColor = LightColor * FinalExposition.x;
	/////////////////////////////////////////////////////////////////////////////////////////
	lit += LightColor;
}

[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]
void cs_main(uint3 groupId          : SV_GroupID,
			 uint3 dispatchThreadId : SV_DispatchThreadID,
			 uint3 groupThreadId	: SV_GroupThreadID
)
{
	// NOTE: This is currently necessary rather than just using SV_GroupIndex to work
	// around a compiler bug on Fermi.
	uint groupIndex = groupThreadId.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadId.x;

	// How many total lights?
	uint totalLights, dummy;
	gLight.GetDimensions(totalLights, dummy);

	//Obtenemos las coordenadas que vamos a trabajar(segun el thread)
	uint2 globalCoords = dispatchThreadId.xy;
	WriteSample(globalCoords, float4(1.0f, 0.0f, 0.0f, 1.0f));
	//Tomamos las muestras de trabajo de los resultados del GBuffer
	SurfaceData surfaceSample = ComputeSurfaceDataFromGBufferSample(globalCoords);
	
	// Work out Z bounds for our samples
	float minZSample = mCameraNearFar.x;
	float maxZSample = mCameraNearFar.y;

	float viewSpaceZ = surfaceSample.position.z;
	bool validPixel = viewSpaceZ >= mCameraNearFar.y && viewSpaceZ <  mCameraNearFar.x;
	[flatten] if (validPixel)
	{
		minZSample = min(minZSample, viewSpaceZ);
		maxZSample = max(maxZSample, viewSpaceZ);
	}
	
	// Initialize shared memory light list and Z bounds
	if (groupIndex == 0) 
	{
		sTileNumLights = 0;
		sNumPerSamplePixels = 0;
		sMinZ = 0x7F7FFFFF;      // Max float
		sMaxZ = 0;
	}

	GroupMemoryBarrierWithGroupSync();

	// NOTE: Can do a parallel reduction here but now that we have MSAA and store sample frequency pixels
	// in shaded memory the increased shared memory pressure actually *reduces* the overall speed of the kernel.
	// Since even in the best case the speed benefit of the parallel reduction is modest on current architectures
	// with typical tile sizes, we have reverted to simple atomics for now.
	// Only scatter pixels with actual valid samples in them
	if (maxZSample >= minZSample) 
	{
		InterlockedMin(sMinZ, asuint(minZSample));
		InterlockedMax(sMaxZ, asuint(maxZSample));
	}

	GroupMemoryBarrierWithGroupSync();
	
	float minTileZ = asfloat(sMinZ);
	float maxTileZ = asfloat(sMaxZ);

	// NOTE: This is all uniform per-tile (i.e. no need to do it per-thread) but fairly inexpensive
	// We could just precompute the frusta planes for each tile and dump them into a constant buffer...
	// They don't change unless the projection matrix changes since we're doing it in view space.
	// Then we only need to compute the near/far ones here tightened to our actual geometry.
	// The overhead of group synchronization/LDS or global memory lookup is probably as much as this
	// little bit of math anyways, but worth testing.

	// Work out scale/bias from [0, 1]
	float2 tileScale = float2(ViewportDimensions.xy) * rcp(float(2 * COMPUTE_SHADER_TILE_GROUP_DIM));
	float2 tileBias = tileScale - float2(groupId.xy);

	// Now work out composite projection matrix
	// Relevant matrix columns for this tile frusta
	float4 c1 = float4(mCameraProj._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
	float4 c2 = float4(0.0f, -mCameraProj._22 * tileScale.y, tileBias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

	// Derive frustum planes
	float4 frustumPlanes[6];
	// Sides
	frustumPlanes[0] = c4 - c1;
	frustumPlanes[1] = c4 + c1;
	frustumPlanes[2] = c4 - c2;
	frustumPlanes[3] = c4 + c2;
	// Near/far
	frustumPlanes[4] = float4(0.0f, 0.0f, 1.0f, -minTileZ);
	frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f, maxTileZ);

	// Normalize frustum planes (near/far already normalized)
	[unroll] for (uint i = 0; i < 4; ++i) 
	{
		frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
	}

	// Cull lights for this tile
	for (uint lightIndex = groupIndex; lightIndex < totalLights; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE) 
	{
		DirectionalLight light = gLight[lightIndex];

		// Cull: point light sphere vs tile frustum
		bool inFrustum = true;
		/*[unroll] for (uint i = 0; i < 6; ++i) 
		{
			float d = dot(frustumPlanes[i], float4(light.position, 1.0f));
			inFrustum = inFrustum && (d >= -light.attenuationEnd);
		}*/

		[branch] if (inFrustum) 
		{
			// Append light to list
			// Compaction might be better if we expect a lot of lights
			uint listIndex;
			InterlockedAdd(sTileNumLights, 1, listIndex);
			sTileLightIndices[listIndex] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();
			
	uint numLights = sTileNumLights;
	
	WriteSample(globalCoords, float4(0.0f, 1.0f, 0.0f, 1.0f));

	// Only process onscreen pixels (tiles can span screen edges)
	if (all(globalCoords < ViewportDimensions.xy))
	{		
		if (numLights > 0)
		{
			float3 lit = float3(0.0f, 0.0f, 0.0f);
			for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex)
			{
				DirectionalLight light = gLight[sTileLightIndices[tileLightIndex]];
				AccumulateBRDF(surfaceSample, light, lit);
			}

			// Write sample 0 result
			WriteSample(globalCoords, float4(lit, 1.0f));
		}
		else 
		{
			// Otherwise no lights affect here so clear all samples
			WriteSample(globalCoords, float4(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}

