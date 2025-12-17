//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuadInterface.hlsl"
Texture2D Texture	: register(t0);

float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{
	return Texture.Sample(DefaultSampler, Input.TexCoord);
}