//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"
Texture2D AlbedoMap			: register(t0);
Texture2D MetallicMap		: register(t1);
Texture2D NormalMap			: register(t2);
Texture2D RoughnessMap		: register(t3);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSGeometry_INPUT vs_main(VSGeometry_INPUT Input)
{
	PSGeometry_INPUT Output = (PSGeometry_INPUT)0;
	Output.Pos = float4(Input.Position.xyz, 1.0);
	Output.Position = Input.Position;

	if (IsStatic == 0)
	{
		float4x4 BoneTransform = float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		for (int i = 0; i < 4; i++)
		{
			BoneTransform += JointMatrix[Input.BoneIndex[i]] * Input.BoneWeight[i];
		}

		Output.Pos = mul(Output.Pos, BoneTransform);
		Output.Position = mul(Output.Position, BoneTransform);

		//Calculamos el espacio de tangente a espacio de mundo usando
		//el world space tangent, la binormal y la nomral como vector base.
		Output.TangentToWorld[0] = mul(mul(float4(Input.Tangent, 0.0f), BoneTransform), matWorld).xyz;
		Output.TangentToWorld[1] = mul(mul(float4(Input.Binormal, 0.0f), BoneTransform), matWorld).xyz;
		Output.TangentToWorld[2] = mul(mul(float4(Input.Normal, 0.0f), BoneTransform), matWorld).xyz;
	}
	else
	{
		//Calculamos el espacio de tangente a espacio de mundo usando
		//el world space tangent, la binormal y la nomral como vector base.
		Output.TangentToWorld[0] = mul(float4(Input.Tangent, 0.0f), matWorld).xyz;
		Output.TangentToWorld[1] = mul(float4(Input.Binormal, 0.0f), matWorld).xyz;
		Output.TangentToWorld[2] = mul(float4(Input.Normal, 0.0f), matWorld).xyz;
	}

	Output.Pos = mul(Output.Pos, matWorld);
	Output.Position = mul(Output.Position, matWorld);
	Output.Position = mul(Output.Position, mCameraViewProj);
	Output.TexCoord = Input.TexCoord;

	return(Output);
}

//--------------------------------------------------------------------------------------
// PS Shader
//--------------------------------------------------------------------------------------
GBuffer_OUTPUT ps_main(PSGeometry_INPUT Input)
{
	GBuffer_OUTPUT Output;

	//Toma de muestras de los mapas de textura 
	float3 albedoColor = AlbedoMap.Sample(DefaultSampler, Input.TexCoord.xy).xyz;
	float3 normal = NormalMap.Sample(DefaultSampler, Input.TexCoord.xy).xyz;
	float metallic = MetallicMap.Sample(DefaultSampler, Input.TexCoord.xy).x;
	float roughness = RoughnessMap.Sample(DefaultSampler, Input.TexCoord.xy).x;

	//Cambio Roughness/glossines
	if (IsRoughness == 0)
	{
		roughness = 1.0f - roughness;
	}

	//Correccion de gama(en espacio de operacion, al finalizar se regresa a la gama lineal)
	albedoColor = pow(albedoColor.xyz, 1.0f / 2.2f);

	//Calculamos las normales a espacio de pantalla
	normal = 2.0f * normal - 1.0f;
	normal = normalize(normal);
	normal = normalize(mul(normal, Input.TangentToWorld));

	//Calculamos la profundidad lineal
	float4 ViewPosition = mul(mCameraView, Input.Pos);
	float Depth = ViewPosition.z / (mCameraNearFar.y - mCameraNearFar.x);

	//Muestras de salida
	Output.GAlbedo_GDepth = float4(albedoColor, Depth);
	Output.GNormal_RoughMetal = float4(EncodeSphereMap(normal), roughness, metallic);
	Output.GPosition = Input.Pos;

	return Output;
}


