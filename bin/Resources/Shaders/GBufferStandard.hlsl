//--------------------------------------------------------------------------------------
// File: GBuffer 
//
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#include "ShaderBuffers.hlsl"
Texture2D AlbedoMap				: register(t0);
Texture2D MetallicMap			: register(t1);
Texture2D NormalMap				: register(t2);
Texture2D RoughnessMap			: register(t3);
TextureCube IrradianceMap		: register(t4);
TextureCube EnvironmentMap		: register(t5);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSGeometry_INPUT vs_main(VSGeometry_INPUT Input )
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
// PS Shader Functions
//--------------------------------------------------------------------------------------

float3 Diffuse(float3 pAlbedo)
{
	return pAlbedo / M_PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
	//GGX ISOTROPICO
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	float denominador = NdH2 * (a2 - 1.0f) + 1.0f;
	denominador *= denominador;
	denominador *= M_PI;

	return a2 / denominador;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
	//GGX Smith Schick
	float K = a * 0.5f; //Constante de absorcion
	float GV = NdV / (NdV * (1 - K) + K); //arametro de distribucion sobre la vista, lo q percibes segun el angulo de vista
	float GL = NdL / (NdL * (1 - K) + K);//segun lo q percives de rebote

	return GV * GL;
}

float3 Fresnel_Schlick(float3 specularColor, float3 v, float3 h)
{
	return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float Specular_D(float a, float NdH) // Distribucion de normales
{
	return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) // Valor geometrico 
{
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Specular_F(float3 specularColor, float3 v, float3 h)
{
	return Fresnel_Schlick(specularColor, v, h);
}

float Specular_F_Roughness(float3 specularColor, float a, float3 h, float3 v)
{
	return (specularColor + (max(1.0f - a, specularColor) - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float3 a, float NdL, float NdV, float NdH, float VdH, float LdV)//Aporte especular llenando el compute light
{
	return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f); // 
}

float3 ComputeLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightPosition, float3 lightColor, float3 lightDir, float3 viewDir)
{
	//Calculamos los vectores del brdf
	float NdL = saturate(dot(normal, lightDir));
	float NdV = saturate(dot(normal, viewDir));
	float3 h = normalize(lightDir + viewDir);
	float NdH = saturate(dot(normal, h));
	float VdH = saturate(dot(viewDir, h));
	float LdV = saturate(dot(lightDir, viewDir));
	float a = max(0.001f, roughness * roughness);//el numero es para seguridad, ya q el resultado se usara como divisor

	float3 ColorDiffuse = Diffuse(albedoColor);
	float3 ColorSpecular = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);

	return lightColor * NdL * (ColorDiffuse * (1.0f - ColorSpecular) + ColorSpecular);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
GBuffer_OUTPUT ps_main(PSGeometry_INPUT Input)
{
	GBuffer_OUTPUT Output;
	Output.Position = Input.Pos;

	//Este es general
	float3 albedoColor = AlbedoMap.Sample(DefaultSampler, Input.TexCoord.xy).xyz;
	float3 normal = NormalMap.Sample(DefaultSampler, Input.TexCoord.xy).xyz;

	//METALLIC _ DISNEY_BRDF
	float metallic = MetallicMap.Sample(DefaultSampler, Input.TexCoord.xy).x;
	float roughness = RoughnessMap.Sample(DefaultSampler, Input.TexCoord.xy).x;

	if (IsRoughness == 0)
	{
		roughness = 1.0f - roughness;
	}	

	Output.Roughness = float4(roughness.xxx, 1.0f);
	Output.Metallic = float4(metallic.xxx, 1.0f);

	//Correccion de gama(en espacio de operacion, al finalizar se regresa a la gama lineal)
	albedoColor = pow(albedoColor.xyz, 2.2f);

	//Calculamos las normales a espacio de pantalla
	normal = 2.0f * normal - 1.0f;
	normal = normalize(normal);
	normal = normalize(mul(normal, Input.TangentToWorld));

	/*
	Output.GNormal_RoughMetal = float4(EncodeSphereMap(normal), 0.0f, 0.0f);
	*/
	Output.Normal = float4(normal, 1.0f);

	//Calculamos la iluminacion
	float4 pos = Input.Pos / Input.Pos.w;
	float3 viewDir = normalize(viewPos.xyz - pos.xyz);
	float3 lightDir = normalize(_DirectionalLight.LightPosition.xyz - pos.xyz);

	//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    METALLIC(especificamente porque usaremos metallic) el numero 0.03 es por el de disney. es un dato de absorcion para la gama q estamos usando aqui
	float3 realAlbedo = albedoColor - albedoColor * metallic;
	float3 realSpecularColor = lerp(0.03f, albedoColor, metallic);

	float3 light1 = ComputeLight(realAlbedo, realSpecularColor, normal, roughness, _DirectionalLight.LightPosition.xyz, _DirectionalLight.LightColor.xyz, lightDir, viewDir);

	//Calculo de vector de reflejo
	float lightDist = length(-_DirectionalLight.LightPosition + pos);
	float3 reflectVector = reflect(-viewDir, normal);

	float mipIndex = roughness * roughness * 8.0f;//valor q se controlara del rougnes del material
	float3 envColor = EnvironmentMap.SampleLevel(DefaultSampler, reflectVector.xyz, 0);//texCUBElod usa el valor w de vector
	float3 irradiance = IrradianceMap.Sample(DefaultSampler, normal);
	envColor = pow(envColor.rgb, 2.2f);

	//Frennel de ambiental
	float3 envFresnel = Specular_F_Roughness(realSpecularColor, roughness * roughness, normal, viewDir);

	//Salida Final
	float4 FinalColor = float4((light1 * FinalExposition.x) + (envFresnel * envColor) + (envFresnel * realAlbedo * irradiance), 1.0f);
	/*
	Output.GAlbedo_GDepth.xyz = (pow(FinalColor.rgb, 2.2f));
	*/
	Output.Albedo = FinalColor;

	float4 ViewPosition = mul(mCameraView, Input.Pos);

	float Depth = ViewPosition.z / (mCameraNearFar.y - mCameraNearFar.x);
	Output.Depth = float4(Depth.xxx, 1.0f);

	return Output;
}
