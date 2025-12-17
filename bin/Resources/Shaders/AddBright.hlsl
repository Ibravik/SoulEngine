//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D BlurTexture	: register(t0);
Texture2D BrightTexture	: register(t1);

float4 ps_main(VSPlane_INPUT Input) : SV_Target
{
	return 0.5f * (BlurTexture.SampleLevel(DefaultSampler, Input.TexCoord, MipLevel.x) + BrightTexture.SampleLevel(DefaultSampler, Input.TexCoord, MipLevel.y));
}

