
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float2 uv			: UV;
};

struct DirectionalLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float3 normal = normalize(input.normal);

	float lightAmount = saturate(dot(normal, normalize(-light.direction)));
	float lightAmount2 = saturate(dot(normal, normalize(-light2.direction)));

	return (light.ambientColor + light2.ambientColor + (light.diffuseColor * lightAmount) + (light2.diffuseColor * lightAmount2)) * surfaceColor;
}