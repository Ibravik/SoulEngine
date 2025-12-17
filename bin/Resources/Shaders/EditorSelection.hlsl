//--------------------------------------------------------------------------------------
// File: Editor Selector 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"
Texture2D ColorMap			: register(t6);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VSPlane_INPUT vs_main(VSGeometry_INPUT Input )
{
	VSPlane_INPUT Output = (VSPlane_INPUT)0;

	Output.Position = Input.Position;

	if (IsStatic == 0.0f)
	{
		matrix BoneTransform = JointMatrix[Input.BoneIndex[0]] * Input.BoneWeight[0];;
		for (int i = 1; i < 4; i++)
		{
			BoneTransform += JointMatrix[Input.BoneIndex[i]] * Input.BoneWeight[i];
		}
		Output.Position = mul(Output.Position, BoneTransform);
	}

	Output.Position = mul(Output.Position, matWorld);
	Output.Position = mul(Output.Position, mCameraViewProj);
	
	Output.TexCoord = Input.TexCoord;

	return(Output);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(VSPlane_INPUT Input) : SV_Target0
{
	return float4(ColorMap.Sample(DefaultSampler, Input.TexCoord).xyz, 0.5);
}


