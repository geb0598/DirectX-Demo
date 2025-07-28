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
	// [-1, 1] -> [0, 1]
	float3 NormalColor = normalize(Input.WorldNormal) * 0.5f + 0.5f;

	return float4(NormalColor, 1.0f);
}
