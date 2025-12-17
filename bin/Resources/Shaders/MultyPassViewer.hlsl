//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D Albedo		: register(t0);
Texture2D Normal		: register(t1);
Texture2D Roughness		: register(t2);
Texture2D Metallic		: register(t3);
Texture2D SSAO			: register(t4);
Texture2D Final			: register(t5);

sampler LinearSampler	: register(s0);

float4 ps_main(VSPlane_INPUT Input) : SV_TARGET
{
	const float Offset = 1.0f / 6.0f;

	if(Input.TexCoord.x < Offset)
	{
		return Albedo.Sample(LinearSampler, Input.TexCoord);
	}
	if (Input.TexCoord.x < Offset * 2 )
	{
		return Normal.Sample(LinearSampler, Input.TexCoord);
	}	
	if (Input.TexCoord.x < Offset * 3)
	{
		return Roughness.Sample(LinearSampler, Input.TexCoord);
	}
	if (Input.TexCoord.x < Offset * 4)
	{
		return Metallic.Sample(LinearSampler, Input.TexCoord);
	}
	if (Input.TexCoord.x < Offset * 5)
	{
		float Color = 1.0f - SSAO.Sample(LinearSampler, Input.TexCoord).x;
		return float4(Color.xxx, 1);
	}	
	else
	{
		return Final.Sample(LinearSampler, Input.TexCoord);
	}
}
	
