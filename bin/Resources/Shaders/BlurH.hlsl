//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D g_Input0	: register(t0);
sampler g_PointSampler : register(s0);

float4 ps_main(VSPlane_INPUT Input) : SV_TARGET
{
	const float2 OFFSETS[] = { float2(-4, 0), float2(-3, 0), float2(-2, 0), float2(-1, 0), float2(0, 0), float2(1, 0), float2(2, 0), float2(3, 0), float2(4, 0) };
	const float texScaler = 1.0f / (ViewportDimensions.x);

	static const float g_BlurWeights[] =
	{
		0.004815026f,
		0.028716039f,
		0.102818575f,
		0.221024189f,
		0.28525234f,
		0.221024189f,
		0.102818575f,
		0.028716039f,
		0.004815026f
	};

	float4
	output =  g_Input0.Load(Input.TexCoord + (OFFSETS[0] * texScaler), MipLevel.x) * g_BlurWeights[0];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[1] * texScaler), MipLevel.x) * g_BlurWeights[1];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[2] * texScaler), MipLevel.x) * g_BlurWeights[2];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[3] * texScaler), MipLevel.x) * g_BlurWeights[3];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[4] * texScaler), MipLevel.x) * g_BlurWeights[4];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[5] * texScaler), MipLevel.x) * g_BlurWeights[5];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[6] * texScaler), MipLevel.x) * g_BlurWeights[6];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[7] * texScaler), MipLevel.x) * g_BlurWeights[7];
	output += g_Input0.Load(Input.TexCoord + (OFFSETS[8] * texScaler), MipLevel.x) * g_BlurWeights[8];

	return output;
}