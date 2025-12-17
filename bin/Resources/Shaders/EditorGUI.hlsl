//--------------------------------------------------------------------------------------
// File: SkyBox
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D texture0;
sampler sampler0;

//--------------------------------------------------------------------------------------
// InputParameters
//--------------------------------------------------------------------------------------
cbuffer cbCamera : register(b0)
{
	float4x4 mCameraProj;
};
           
struct VS_INPUT
{
	float2 pos : POSITION;
	float2 uv  : TEXCOORD0;
	float4 col : COLOR0;	
};
            
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};
         
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output;
	output.pos = mul(mCameraProj, float4(input.pos.xy, 0.f, 1.f));
	output.col = input.col;
	output.uv = input.uv;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(PS_INPUT input) : SV_Target0
{
	float4 out_col = input.col * texture0.Sample(sampler0, input.uv);
	return out_col;
}

