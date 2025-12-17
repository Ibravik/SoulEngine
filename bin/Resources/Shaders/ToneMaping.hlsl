//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ScreenAlignedQuad.hlsl"
Texture2D BlurMap						: register(t0);
Texture2D SSAOMap						: register(t1);
StructuredBuffer<uint2> gLitTextureFlat : register(t2);

//--------------------------------------------------------------------------------------
// Shader Functions Tone
//--------------------------------------------------------------------------------------

float4 BasicExposure(in float3 Color, in float exposure)
{
	float4 retColor = float4(Color, 1);
	retColor *= exposure;
	retColor = pow(retColor, 1.0f / 2.2f);

	return retColor;
}

float4 Reinhard(in float3 Color, in float exposure)
{
	float4 retColor = float4(Color, 1);
	retColor *= exposure;
	retColor = retColor / (1.0f + retColor);
	retColor = pow(retColor, 1.0f / 2.2f);

	return retColor;
}

float4 Haarm_Peter_Duiker(in float3 Color, in float exposure)
{
	float3 texColor = Color;
	texColor *= exposure;

	float3 ld = 0.002f;
	float linReference = 0.18;
	float logReference = 444;
	float logGamma = 0.45f;

	float3 LogColor;
	LogColor.rgb = (log10(0.4f * texColor.rgb / linReference) / ld * logGamma + logReference) / 1023.f;
	LogColor.rgb = saturate(LogColor.rgb);

	float FilmLutWidth = 256;
	float Padding = 0.5f / FilmLutWidth;

	float3 retColor;
	retColor.r = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).r;
	retColor.g = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.g), 0.5f)).r;
	retColor.b = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.b), 0.5f)).r;

	return float4(retColor, 1);
}

float3 Burgess_Dawson(in float3 Color, in float exposure)
{
	float3 texColor = Color;
	texColor *= exposure;
	float3 x = max(0, texColor - 0.004);
	float3 retColor = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return retColor;
}

float3 Uncharted2Tonemap(float3 x)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;
	
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float4 Uncharted2(in float3 Color, in float exposure)
{
	float W = 11.2f;
	float3 texColor = Color;
	texColor *= exposure;
	float ExposureBias = 2.0f;
	float3 curr = Uncharted2Tonemap(ExposureBias * texColor);

	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	float3 color = curr * whiteScale;

	float3 retColor = pow(color, 1 / 2.2);
	return float4(retColor, 1);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{
	//Salida Final	
	float3 FinalColor = UnpackRGBA16(gLitTextureFlat[GetFramebufferSampleAddress(Input.Position.xy)]).xyz;

	FinalColor = Reinhard(FinalColor, BloomMuiltiplier_Exposure.y);

	FinalColor *= SSAOMap.Sample(DefaultSampler, Input.TexCoord.xy).xyz;

	FinalColor += BloomMuiltiplier_Exposure.x * BlurMap.SampleLevel(DefaultSampler, Input.TexCoord.xy, 0).xyz;
	
	return float4(FinalColor,  1.0f);
}
