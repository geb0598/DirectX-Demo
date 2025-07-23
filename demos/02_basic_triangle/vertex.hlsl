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

	output.position = input.position;
	output.color = float4(1.0f, 0.0f, 0.0f, 1.0f); // TODO: temporary red color

	return output;
}