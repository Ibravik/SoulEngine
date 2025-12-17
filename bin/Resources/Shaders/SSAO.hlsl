//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D NormalMap 			: register( t0 );
Texture2D PositionMap			: register( t1 );

//--------------------------------------------------------------------------------------
// Shader Functions
//--------------------------------------------------------------------------------------
float4 GetPosition(in float2 uv)//el in es para indicar parametro de entrada, out sera para salida
{
	return PositionMap.Sample(DefaultSampler, uv);
}

float3 GetNormal(in float2 uv)//el in es para indicar parametro de entrada, out sera para salida
{
	float3 DecodeNormal = DecodeSphereMap(NormalMap.Sample(DefaultSampler, uv).xy);
	
	return normalize(DecodeNormal * 2.0f - 1.0f);
}

float2 GetRandom(in float2 uv)//Esta es la distribucion de morntecarlo
{
	float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f)*1.0f))*46336.23745f));
	float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f)*2.0f))*34748.34744f));
	float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f)*3.0f))*59998.47362f));

	return normalize(float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(in float2 tcoord, in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = GetPosition(tcoord + uv) - p;
	const float3 v = normalize(diff);
	const float d = length(diff) * Intensity_Scale_Bias_SampleRadius.y;
	return max(0.0f, dot(cnorm, v) - Intensity_Scale_Bias_SampleRadius.z)*(1.0 / (1.0 + d)) * Intensity_Scale_Bias_SampleRadius.x;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{	
	float4 Color = float4(1, 0, 0, 1);

	const float2 vec[4] =
	{
		float2(1,0), float2(-1,0), float2(0,1), float2(0,-1)
	};

	float4 p = GetPosition(Input.TexCoord);

	float3 n = GetNormal(Input.TexCoord);
	float2 rand = GetRandom(Input.TexCoord);

	float ao = 0.0f;
	float rad = Intensity_Scale_Bias_SampleRadius.w / p.z;

	int iterations = 4;
	for (int j = 0; j < iterations; ++j)
	{
		float2 coord1 = reflect(vec[j], rand) * rad;
		float2 coord2 = float2(coord1.x*0.707 - coord1.y * 0.707,
			coord1.x*0.707 + coord1.y * 0.707);

		ao += DoAmbientOcclusion(Input.TexCoord, coord1 * 0.25, p.xyz, n);
		ao += DoAmbientOcclusion(Input.TexCoord, coord2 * 0.5, p.xyz, n);
		ao += DoAmbientOcclusion(Input.TexCoord, coord1 * 0.75, p.xyz, n);
		ao += DoAmbientOcclusion(Input.TexCoord, coord2, p.xyz, n);
	}

	ao /= (iterations * 4);

	Color.rgb = 1 - ao.xxx;
	
	return Color;
}
