//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"

Texture2D Texture	: register(t0);

PS_VRDistortion vs_main(VSGeometry_INPUT In)
{
	PS_VRDistortion Out;
	Out.Position = In.Position;
	Out.TexCoord = In.TexCoord;
	Out.TexCoord1 = In.TexCoord1;
	Out.TexCoord2 = In.TexCoord2;

	return Out;
}

float4 ps_main(PS_VRDistortion Input) : SV_Target0
{
	float4 Color;	
	float fBoundsCheck = ((dot(float2(min(Input.TexCoord1.xy, float2(0.05, 0.05))), float2(1.0, 1.0)) + dot(float2(max(Input.TexCoord1.xy, float2(0.95, 0.95))), float2(1.0, 1.0))));

	if( fBoundsCheck > 1.0 )
	{ 
		Color = float4(0, 0, 0, 1.0f);
	}
	else
	{
		Texture.Sample(DefaultSampler, Input.TexCoord).x;
		float red = Texture.Sample(DefaultSampler, Input.TexCoord).x;
		float green = Texture.Sample(DefaultSampler, Input.TexCoord1).y;
		float blue = Texture.Sample(DefaultSampler, Input.TexCoord2).z;
		Color = float4(red, green, blue, 1.0);
	}

	return Color;  
}
