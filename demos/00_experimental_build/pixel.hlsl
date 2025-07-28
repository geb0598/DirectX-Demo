cbuffer MATERIAL_DATA : register(b1)
{
	float4 M_DiffuseColor;
	float4 M_SpecularColor;
	float4 M_AmbientColor;
	float4 Shininess;
 };

cbuffer LIGHTING_DATA : register(b2)
{
	float4 L_DiffuseColor;
	float4 L_SpecularColor;
	float4 L_AmbientColor;
	float4 LightingPosition;
	float4 CameraPosition;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
	// ---------------------
	// 1. Normalization
	// ---------------------
	float3 Normal = normalize(Input.WorldNormal);
	float3 View = normalize(CameraPosition - Input.WorldPosition);
	float3 Lighting = normalize(LightingPosition - Input.WorldPosition);

	// ---------------------
	// 2. Diffuse Term
	// ---------------------
	float3 DiffuseColor = max(dot(Normal, Lighting), 0.0f) * M_DiffuseColor * L_DiffuseColor;

	// ---------------------
	// 3. Specular Term
	// ---------------------
	float3 Reflection = 2.0f * Normal * dot(Normal, Lighting) - Lighting;
	// TODO: Why opposite direction?
	// float3 Reflection = reflect(Lighting, Normal);
	float3 SpecularColor = pow(max(dot(Reflection, View), 0.0f), Shininess.x) * M_SpecularColor * L_SpecularColor;

	// ---------------------
	// 4. Ambient Term
	// ---------------------
	float3 AmbientColor = M_AmbientColor * L_AmbientColor;

	// return float4(saturate(DiffuseColor), 1.0f);
	return float4(DiffuseColor + SpecularColor + AmbientColor, 1.0f);

	// 1. 노멀 벡터를 [-1, 1]에서 [0, 1] 범위로 매핑
	// float3 normalColor = normalize(Input.WorldNormal) * 0.5f + 0.5f;

	// 2. 알파 값은 1.0으로 설정
	// return float4(normalColor, 1.0f);
}
