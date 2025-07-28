cbuffer TRANSFORM_DATA : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float4 DepthPosition : TEXCOORD2;
};

PS_INPUT mainVS(VS_INPUT Input)
{
	PS_INPUT Output;

	Output.WorldPosition = mul(World, Input.Position);
	float4 ViewPosition = mul(View, Output.WorldPosition);
	Output.Position = mul(Projection, ViewPosition);
	Output.DepthPosition = Output.Position;

	// TODO: No inverse() in HLSL?
	// Output.WorldNormal = normalize(mul(Input.Normal, (float3x3)transpose(inverse(World))));

	Output.WorldNormal = normalize(mul((float3x3)World, Input.Normal));

	return Output;
}