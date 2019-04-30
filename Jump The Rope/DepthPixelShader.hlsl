#include "../EngineCore/LightTypes.h"

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
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: UV;
	float3 worldPos		: POSITION;
};

cbuffer externalData : register(b0)
{
	Lights lights;
	float3 cameraPos;
	float2 uvOffset;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

Texture2D normalTexture : register(t1);


float4 main(VertexToPixel input) : SV_TARGET
{

	float depth = ((input.position.z - 1) * 75) + 1.1;
	depth = clamp(depth, 0, 1);
	return float4(depth, depth, depth, 1.0);
	//return float4(1.0,0.0,0.0,1.0);
}