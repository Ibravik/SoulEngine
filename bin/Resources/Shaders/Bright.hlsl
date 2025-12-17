//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D LuminanceBuffer	: register(t0);

float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{
	return max(LuminanceBuffer.SampleLevel(DefaultSampler, Input.TexCoord.xy, 0) - BloomThreshold.x, (float4)0.0f);
}

