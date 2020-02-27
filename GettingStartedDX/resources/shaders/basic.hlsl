// Constant Buffer is currenly only accesable from the
// vertex shader.
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Proj;
};

// VS_Input is a representation of the vertices passed to the shader.
// All data for a vertex in the vertexbuffer must be represented here
// in sequencial order.
struct VS_Input
{
	float3 position : POSITION;
	float3 color	: COLOR;
};

// PS_Input 
// These are set and returned in the VertexShader and passed
// on to the PixelShader after they have been interpolated by
// by the rasterizer
struct PS_Input
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};


PS_Input vs_main(const VS_Input input)
{
	PS_Input output = (PS_Input)0;

	output.position = mul(World, float4(input.position.xyz, 1.0f));
	output.position = mul(View, output.position);
	output.position = mul(Proj, output.position);

	output.color = float4(input.color.rgb, 1.0);

	return output;
};

float4 ps_main(PS_Input input) : SV_TARGET
{
	return float4(input.color.xyz, 1.0f);
}