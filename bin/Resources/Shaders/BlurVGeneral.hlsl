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

float4 ps_main(float4 pos : SV_POSITION) : SV_TARGET
{
	const float2 OFFSETS[] = {float2(0, -4), float2(0, -3), float2(0, -2), float2(0, -1), float2(0, 0), float2(0, 1), float2(0, 2), float2(0, 3), float2(0, 4)};

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
	output  = g_Input0.Load(float3(pos.xy + OFFSETS[0], MipLevel.x)) * g_BlurWeights[0];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[1], MipLevel.x)) * g_BlurWeights[1];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[2], MipLevel.x)) * g_BlurWeights[2];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[3], MipLevel.x)) * g_BlurWeights[3];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[4], MipLevel.x)) * g_BlurWeights[4];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[5], MipLevel.x)) * g_BlurWeights[5];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[6], MipLevel.x)) * g_BlurWeights[6];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[7], MipLevel.x)) * g_BlurWeights[7];
	output += g_Input0.Load(float3(pos.xy + OFFSETS[8], MipLevel.x)) * g_BlurWeights[8];

	return output;
}