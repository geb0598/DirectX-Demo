cbuffer VS_CONSTANT_BUFFER_DATA : register(b0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
};

struct VS_INPUT
{
	float4 position : POSITION;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;

	float4 world_position = mul(input.position, World);
	float4 view_position = mul(world_position, View);
	float4 clip_position = mul(view_position, Projection);

	output.position = clip_position;
	output.color = float4(1.0f, 0.0f, 0.0f, 1.0f); // TODO: temporary red color

	return output;
}