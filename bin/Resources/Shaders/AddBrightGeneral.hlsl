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
Texture2D g_Input1	: register(t1);
Texture2D g_Input2	: register(t2);

float4 ps_main(VSPlane_INPUT Input) : SV_Target
{
	return 0.7f * ((0.5f * (g_Input0.SampleLevel(DefaultSampler, Input.TexCoord, MipLevel.x) + g_Input1.SampleLevel(DefaultSampler, Input.TexCoord, MipLevel.x))) + g_Input2.Load(float3(Input.Position.xy, 0)));
}