//--------------------------------------------------------------------------------------
// Constant Buffer constants
//--------------------------------------------------------------------------------------
#define M_PI 3.14159265359f;

//--------------------------------------------------------------------------------------
// Inputs & Outputs VS PS
//--------------------------------------------------------------------------------------
struct VSGeometry_INPUT
{
	float4 Position				: POSITION0;
	float2 TexCoord				: TEXCOORD0;
	float3 Normal				: NORMAL;
	float3 Binormal				: BINORMAL;
	float3 Tangent				: TANGENT;
	uint4 BoneIndex				: BLENDINDICES;
	float4 BoneWeight			: BLENDWEIGHT;
};

struct PSGeometry_INPUT
{
	float4 Position				: SV_POSITION0;
	float2 TexCoord				: TEXCOORD0;
	float3x3 TangentToWorld		: TEXCOORD2;
	float4 Pos					: NORMAL1;
};

/*
struct GBuffer_OUTPUT
{
	float4 GAlbedo_GDepth		: SV_Target0;
	float4 GNormal_RoughMetal	: SV_Target1;
	float4 GPosition			: SV_Target2;
};
*/

struct GBuffer_OUTPUT
{
	float4 Albedo				: SV_Target0;
	float4 Normal				: SV_Target1;
	float4 Position				: SV_Target2;
	float4 Roughness			: SV_Target3;
	float4 Metallic				: SV_Target4;
	float4 Depth				: SV_Target5;
};

struct VSPlane_INPUT
{
	float4 Position				: SV_POSITION;
	float2 TexCoord				: TEXCOORD0;
};

struct PS_VRDistortion
{
	float4 Position				: SV_POSITION0;
	float2 TexCoord				: TEXCOORD0;
	float2 TexCoord1			: TEXCOORD1;
	float2 TexCoord2			: TEXCOORD2;
};

struct PSSkyBox_INPUT
{
	float4 Position				: SV_POSITION0;
	float3 SkyBoxCoord          : NORMAL0;
};

SamplerState DefaultSampler;

//--------------------------------------------------------------------------------------
// Constant Buffers Mesh
//--------------------------------------------------------------------------------------
cbuffer FrameBuffer
{
	matrix mCameraViewProj;
	matrix mCameraView;
	matrix mCameraProj;
	float4 viewPos;
	float4 mCameraNearFar;
	uint4 ViewportDimensions;
};

//--------------------------------------------------------------------------------------
// Constant Buffers Luces
//--------------------------------------------------------------------------------------
struct DirectionalLight
{
	float4 LightPosition;
	float4 LightColor;
};

cbuffer cbDirectionalLight
{
	DirectionalLight _DirectionalLight;
};

//--------------------------------------------------------------------------------------
// Constant Buffers PBR
//--------------------------------------------------------------------------------------
cbuffer cbPBR
{	
	float4 FinalExposition;
	float4 Intensity_Scale_Bias_SampleRadius;
	float4 BloomThreshold;
	float4 BloomMuiltiplier_Exposure;
	int4 MipLevel;
};

//--------------------------------------------------------------------------------------
// Constant Buffers caracteristicas del modelo
//--------------------------------------------------------------------------------------
cbuffer cbModel
{
	matrix matWorld;
};

cbuffer cbSkeletal
{
	matrix JointMatrix[120];
};

cbuffer cbStaticModel
{
	uint IsStatic;
};

cbuffer cbRougthnessMaterial
{
	uint IsRoughness;
};

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 position;         // View space position
	float3 normal;               // View space normal
	float4 albedo;				 // Color 
	float Roughness;        // Treated as a multiplier on albedo
	float Metallic;		 //
};

//--------------------------------------------------------------------------------------
// Funciones repetitivas
//--------------------------------------------------------------------------------------
float2 EncodeSphereMap(float3 n)
{
	float oneMinusZ = 1.0f - n.z;
	float p = sqrt(n.x * n.x + n.y * n.y + oneMinusZ * oneMinusZ);
	return n.xy / p * 0.5f + 0.5f;
}

float3 DecodeSphereMap(float2 e)
{
	float2 tmp = e - e * e;
	float f = tmp.x + tmp.y;
	float m = sqrt(4.0f * f - 1.0f);

	float3 n;
	n.xy = m * (e * 4.0f - 2.0f);
	n.z = 3.0f - 8.0f * f;
	return n;
}

// - RGBA 16-bit per component packed into a uint2 per texel
float4 UnpackRGBA16(uint2 e)
{
	return float4(f16tof32(e), f16tof32(e >> 16));
}

uint2 PackRGBA16(float4 c)
{
	return f32tof16(c.rg) | (f32tof16(c.ba) << 16);
}

// Linearize the given 2D address + sample index into our flat framebuffer array
uint GetFramebufferSampleAddress(uint2 coords)
{
	// Major ordering: Row (x), Col (y), MSAA sample
	return (coords.y * ViewportDimensions.x) + coords.x;
}