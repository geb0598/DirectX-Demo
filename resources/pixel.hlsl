cbuffer PS_CONSTANT_BUFFER_DATA : register(b1)
{
	float4 LightPosition;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 mainPS(PS_INPUT input) : SV_TARGET
{
	return input.color;
}