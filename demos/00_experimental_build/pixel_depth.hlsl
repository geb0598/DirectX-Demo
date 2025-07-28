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
	float4 DepthPosition : TEXCOORD2;
};

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
	float DepthValue = Input.DepthPosition.z / Input.DepthPosition.w;
	float4 Color;

	if (DepthValue < 0.9f)
	{
		Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	if (DepthValue > 0.95f)
	{
		Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	if (DepthValue > 0.975f)
	{
		Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	return Color;
}
