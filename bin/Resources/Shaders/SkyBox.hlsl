//--------------------------------------------------------------------------------------
// File: SkyBox
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"
TextureCube SkyBoxMap			: register(t0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSSkyBox_INPUT vs_main(VSGeometry_INPUT Input )
{
	PSSkyBox_INPUT Output = (PSSkyBox_INPUT)0;

	Output.Position = mul(Input.Position, matWorld);
	Output.Position = mul(Output.Position, mCameraViewProj);
	Output.SkyBoxCoord = Input.Position.xyz;

	return(Output);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(PSSkyBox_INPUT Input) : SV_Target0
{
	return float4(SkyBoxMap.Sample(DefaultSampler, Input.SkyBoxCoord).xyz, 1.0f);
}


