#ifndef COMPUTE_SHADER_TILE_HLSL
#define COMPUTE_SHADER_TILE_HLSL
//--------------------------------------------------------------------------------------
// File: ComputeLight 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"
#include "ShaderDefines.h"

Texture2D ColorBuffer			: register(t0);
Texture2D GNormals				: register(t1);
Texture2D GPosition				: register(t2);
Texture2D GDepth				: register(t3);

StructuredBuffer<PointLight> gLight : register(t0);
RWStructuredBuffer<uint2> gFramebuffer : register(u0);

groupshared uint sMinZ;
groupshared uint sMaxZ;

// Light list for the tile
groupshared uint sTileLightIndices[MAX_LIGHTS];
groupshared uint sTileNumLights;

// List of pixels that require per-sample shading
// We encode two 16-bit x/y coordinates in one uint to save shared memory space
groupshared uint sPerSamplePixels[COMPUTE_SHADER_TILE_GROUP_SIZE];
groupshared uint sNumPerSamplePixels;


float3 DecodeSphereMap(float2 e)
{
	float2 tmp = e - e * e;
	float f = tmp.x + tmp.y;
	float m = sqrt(4.0f * f - 1.0f);

	float3 n;
	n.xy = m * (e * 4.0f - 2.0f);
	n.z = 3.0f - 8.0f * f;
	return n;
}

SurfaceData ComputeSurfaceDataFromGBufferSample(uint2 positionViewport, uint sampleIndex)
{
	// Load the raw data from the GBuffer
	GBufferOutPut rawData;
	rawData.GNormals = GNormals.Load(int3(positionViewport.xy,1), sampleIndex).xyzw;
	rawData.ColorBuffer = ColorBuffer.Load(int3(positionViewport.xy, 1), sampleIndex).xyzw;
	rawData.GPosition = GPosition.Load(int3(positionViewport.xy, 1), sampleIndex).xyzw;
	float zBuffer = GDepth.Load(int3(positionViewport.xy, 1), sampleIndex).x;

	float2 gbufferDim;
	uint dummy;
	GNormals.GetDimensions(1, gbufferDim.x, gbufferDim.y, dummy);

	// Compute screen/clip-space position and neighbour positions
	// NOTE: Mind DX11 viewport transform and pixel center!
	// NOTE: This offset can actually be precomputed on the CPU but it's actually slower to read it from
	// a constant buffer than to just recompute it.
	float2 screenPixelOffset = float2(2.0f, -2.0f) / gbufferDim;
	float2 positionScreen = (float2(positionViewport.xy) + 0.5f) * screenPixelOffset.xy + float2(-1.0f, 1.0f);
	float2 positionScreenX = positionScreen + float2(screenPixelOffset.x, 0.0f);
	float2 positionScreenY = positionScreen + float2(0.0f, screenPixelOffset.y);

	// Decode into reasonable outputs
	SurfaceData data;

	// Unproject depth buffer Z value into view space
	float viewSpaceZ = mCameraProj._43 / (zBuffer - mCameraProj._33);

	data.positionView = rawData.GPosition.xyz;
	data.positionViewDX = rawData.GPosition.xyz - data.positionView;
	data.positionViewDY = rawData.GPosition.xyz - data.positionView;

	data.normal = DecodeSphereMap(rawData.GNormals.xy);
	data.albedo = rawData.ColorBuffer;

	data.specularAmount = rawData.GNormals.z;
	data.specularPower = rawData.GNormals.w;

	return data;
}

void ComputeSurfaceDataFromGBufferAllSamples(uint2 positionViewport,
	out SurfaceData surface[1])
{
	// Load data for each sample
	// Most of this time only a small amount of this data is actually used so unrolling
	// this loop ensures that the compiler has an easy time with the dead code elimination.
	[unroll] for (uint i = 0; i < 1; ++i) {
		surface[i] = ComputeSurfaceDataFromGBufferSample(positionViewport, i);
	}
}

// - RGBA 16-bit per component packed into a uint2 per texel
float4 UnpackRGBA16(uint2 e)
{
	return float4(f16tof32(e), f16tof32(e >> 16));
}
uint2 PackRGBA16(float4 c)
{
	return f32tof16(c.rg) | (f32tof16(c.ba) << 16);
}

// Linearize the given 2D address + sample index into our flat framebuffer array
uint GetFramebufferSampleAddress(uint2 coords, uint sampleIndex)
{
	// Major ordering: Row (x), Col (y), MSAA sample
	return (sampleIndex * ViewportDimensions.y + coords.y) * ViewportDimensions.x + coords.x;
}

//--------------------------------------------------------------------------------------
// Utility for writing to our flat MSAAed UAV
void WriteSample(uint2 coords, uint sampleIndex, float4 value)
{
	gFramebuffer[GetFramebufferSampleAddress(coords, sampleIndex)] = PackRGBA16(value);
}

// Pack and unpack two <=16-bit coordinates into a single uint
uint PackCoords(uint2 coords)
{
	return coords.y << 16 | coords.x;
}
uint2 UnpackCoords(uint coords)
{
	return uint2(coords & 0xFFFF, coords >> 16);
}

bool RequiresPerSampleShading(SurfaceData surface[1])
{
	bool perSample = false;

	const float maxZDelta = abs(surface[0].positionViewDX.z) + abs(surface[0].positionViewDY.z);
	const float minNormalDot = 0.99f;        // Allow ~8 degree normal deviations

	[unroll] for (uint i = 1; i < 1; ++i) {
		// Using the position derivatives of the triangle, check if all of the sample depths
		// could possibly have come from the same triangle/surface
		perSample = perSample ||
			abs(surface[i].positionView.z - surface[0].positionView.z) > maxZDelta;

		// Also flag places where the normal is different
		perSample = perSample ||
			dot(surface[i].normal, surface[0].normal) < minNormalDot;
	}

	return perSample;
}

[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]
void ComputeShaderTileCS(uint3 groupId          : SV_GroupID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint3 groupThreadId : SV_GroupThreadID
)
{
	// NOTE: This is currently necessary rather than just using SV_GroupIndex to work
	// around a compiler bug on Fermi.
	uint groupIndex = groupThreadId.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadId.x;

	// How many total lights?
	uint totalLights, dummy;
	gLight.GetDimensions(totalLights, dummy);

	uint2 globalCoords = dispatchThreadId.xy;

	SurfaceData surfaceSamples[1];
	ComputeSurfaceDataFromGBufferAllSamples(globalCoords, surfaceSamples);

	// Work out Z bounds for our samples
	float minZSample = mCameraNearFar.y;
	float maxZSample = mCameraNearFar.x;
	{
		[unroll] for (uint sample = 0; sample < 1; ++sample) {
			// Avoid shading skybox/background or otherwise invalid pixels
			float viewSpaceZ = surfaceSamples[sample].positionView.z;
			bool validPixel =
				viewSpaceZ >= mCameraNearFar.x &&
				viewSpaceZ <  mCameraNearFar.y;
			[flatten] if (validPixel) {
				minZSample = min(minZSample, viewSpaceZ);
				maxZSample = max(maxZSample, viewSpaceZ);
			}
		}
	}

	// Initialize shared memory light list and Z bounds
	if (groupIndex == 0) {
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
	if (maxZSample >= minZSample) {
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
	[unroll] for (uint i = 0; i < 4; ++i) {
		frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
	}

	// Cull lights for this tile
	for (uint lightIndex = groupIndex; lightIndex < totalLights; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE) {
		PointLight light = gLight[lightIndex];

		// Cull: point light sphere vs tile frustum
		bool inFrustum = true;
		[unroll] for (uint i = 0; i < 6; ++i) {
			float d = dot(frustumPlanes[i], float4(light.positionView, 1.0f));
			inFrustum = inFrustum && (d >= -light.attenuationEnd);
		}

		[branch] if (inFrustum) {
			// Append light to list
			// Compaction might be better if we expect a lot of lights
			uint listIndex;
			InterlockedAdd(sTileNumLights, 1, listIndex);
			sTileLightIndices[listIndex] = lightIndex;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	uint numLights = sTileNumLights;

	// Only process onscreen pixels (tiles can span screen edges)
	if (all(globalCoords < ViewportDimensions.xy)) 
	{		
		if (numLights > 0) {
			bool perSampleShading = RequiresPerSampleShading(surfaceSamples);
			[branch] if (perSampleShading) 
			{
				[unroll] for (uint sample = 0; sample < 1; ++sample) {
					WriteSample(globalCoords, sample, float4(1, 0, 0, 1));
				}
			}
			else {
				float3 lit = float3(0.0f, 0.0f, 0.0f);
				for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
					PointLight light = gLight[sTileLightIndices[tileLightIndex]];
					AccumulateBRDF(surfaceSamples[0], light, lit);
				}

				// Write sample 0 result
				WriteSample(globalCoords, 0, float4(lit, 1.0f));

				[branch] if (perSampleShading) {
#if DEFER_PER_SAMPLE
					// Create a list of pixels that need per-sample shading
					uint listIndex;
					InterlockedAdd(sNumPerSamplePixels, 1, listIndex);
					sPerSamplePixels[listIndex] = PackCoords(globalCoords);
#else
					// Shade the other samples for this pixel
					for (uint sample = 1; sample < 1; ++sample) {
						float3 litSample = float3(0.0f, 0.0f, 0.0f);
						for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
							PointLight light = gLight[sTileLightIndices[tileLightIndex]];
							AccumulateBRDF(surfaceSamples[sample], light, litSample);
						}
						WriteSample(globalCoords, sample, float4(litSample, 1.0f));
					}
#endif
				}
				else {
					// Otherwise per-pixel shading, so splat the result to all samples
					[unroll] for (uint sample = 1; sample < 1; ++sample) {
						WriteSample(globalCoords, sample, float4(lit, 1.0f));
					}
				}
			}
		}
		else {
			// Otherwise no lights affect here so clear all samples
			[unroll] for (uint sample = 0; sample < 1; ++sample) {
				WriteSample(globalCoords, sample, float4(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
	}

#if DEFER_PER_SAMPLE && 1 > 1
	// NOTE: We were careful to write only sample 0 above if we are going to do sample
	// frequency shading below, so we don't need a device memory barrier here.
	GroupMemoryBarrierWithGroupSync();

	// Now handle any pixels that require per-sample shading
	// NOTE: Each pixel requires 1 - 1 additional shading passes
	const uint shadingPassesPerPixel = 1 - 1;
	uint globalSamples = sNumPerSamplePixels * shadingPassesPerPixel;

	for (uint globalSample = groupIndex; globalSample < globalSamples; globalSample += COMPUTE_SHADER_TILE_GROUP_SIZE) {
		uint listIndex = globalSample / shadingPassesPerPixel;
		uint sampleIndex = globalSample % shadingPassesPerPixel + 1;        // sample 0 has been handled earlier

		uint2 sampleCoords = UnpackCoords(sPerSamplePixels[listIndex]);
		SurfaceData surface = ComputeSurfaceDataFromGBufferSample(sampleCoords, sampleIndex);

		float3 lit = float3(0.0f, 0.0f, 0.0f);
		for (uint tileLightIndex = 0; tileLightIndex < numLights; ++tileLightIndex) {
			PointLight light = gLight[sTileLightIndices[tileLightIndex]];
			AccumulateBRDF(surface, light, lit);
		}
		WriteSample(sampleCoords, sampleIndex, float4(lit, 1.0f));
	}
#endif
}

#endif // COMPUTE_SHADER_TILE_HLSL
