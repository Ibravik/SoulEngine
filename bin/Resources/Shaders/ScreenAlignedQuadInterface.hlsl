
#include "ShaderBuffers.hlsl"

struct VS_INPUT
{
	float4 Position : SV_POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION0;
	float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT In)
{
	VS_OUTPUT Out;

	Out.Position = float4(sign(In.Position.xy), 0.0, 1.0);
	Out.TexCoord.x = (Out.Position.x + 1.0) * 0.5;
	Out.TexCoord.y = 1.0 - ((Out.Position.y + 1.0) * 0.5);

	Out.Position = mul(Out.Position, matWorld);

	if (IsStatic != 0.0f)
	{
		Out.Position = mul(Out.Position, mCameraViewProj);
	}	

	return Out;

}