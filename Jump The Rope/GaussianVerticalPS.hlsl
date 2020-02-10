
cbuffer Data : register(b0)
{
	float pixelHeight;
	float blurAmount;
	float dofAmount;
}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D Pixels		: register(t0);
Texture2D Depth			: register(t1);
SamplerState Sampler	: register(s0);

static const float KernelOffsets[5] = { 0.f, 1.3846153846f, 3.2307692308f, 5.538461539f,8.307692308f};
static const float BlurWeights[5] = {0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 textureColor = float3(1.f, 0.f, 0.f);
	float2 uv = input.uv;

	float depth = Depth.Sample(Sampler, input.uv).x;

	textureColor = Pixels.Sample(Sampler, uv).xyz * BlurWeights[0];

	for (int i = 1; i < 5; i++)
	{
		float2 normalizedOffset = float2(0.f, KernelOffsets[i] * blurAmount * (distance(depth, 0.65) * dofAmount)) / pixelHeight;
		textureColor += Pixels.Sample(Sampler, uv + normalizedOffset).xyz * BlurWeights[i];
		textureColor += Pixels.Sample(Sampler, uv - normalizedOffset).xyz * BlurWeights[i];
	}


	return float4(textureColor / 2, 1.0);
	//return Depth.Sample(Sampler, input.uv);
}