//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D ColorMap	: register(t0);

float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{
	float4 LuminanceFactor = float4(0.2f, 0.587f, 0.15f, 1.0f);

	float4 Color = float4(ColorMap.Sample(DefaultSampler, Input.TexCoord).xyz, 1.0f);
	float4 luminance = log(dot(Color, LuminanceFactor) + 0.00000001f);

	return(float4(luminance.xxxx));
}

