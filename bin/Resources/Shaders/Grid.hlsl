//--------------------------------------------------------------------------------------
// File: Editor Selector 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSSkyBox_INPUT vs_main(VSPlane_INPUT Input )
{
	PSSkyBox_INPUT Output = (PSSkyBox_INPUT)0;

	Output.Position = mul(Input.Position, matWorld);
	Output.Position = mul(Output.Position, mCameraViewProj);
	Output.Pos = Input.Position.xyz;
	
	Output.TexCoord = Input.TexCoord;

	return(Output);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(PSSkyBox_INPUT Input) : SV_Target0
{
	return float4(ColorMap.Sample(DefaultSampler, Input.Pos).xyz, 1.0f);
}


