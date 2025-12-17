//--------------------------------------------------------------------------------------
// File: ScreenAlignedQuad 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"

VSPlane_INPUT vs_main(VSPlane_INPUT In)
{
	VSPlane_INPUT Out;
	Out.Position = In.Position;
	Out.TexCoord = In.TexCoord; 
	Out.TexCoord.x = 1 - In.TexCoord.x;

	return Out;
}