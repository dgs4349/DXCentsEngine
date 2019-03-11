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
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float2 uv			: UV;
	float3 worldPos		: POSITION;
};

cbuffer externalData : register(b0)
{
	Lights lights;
	float3 cameraPos;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

float Attenuate(PointLight light, float3 worldPos)
{
	float dist = distance(light.position, worldPos);

	// Ranged-based attenuation
	float att = saturate(1.0f - (dist * dist / (light.range * light.range)));

	// Soft falloff
	return att * att;
}

float3 CalcDirLight(DirectionalLight aLight, float3 norm)
{
	float3 lightNormDir = normalize(-aLight.direction.rgb);

	float NdotL = saturate(dot(norm, lightNormDir));

	return float3(aLight.color.rgb * NdotL);
}

float3 CalcPointLight(PointLight light, float3 normal, float3 worldPos, float3 camPos, float3 surfaceColor)
{
	// Calc light direction
	float3 toLight = normalize(light.position - worldPos);
	float3 toCam = normalize(camPos - worldPos);

	// Calculate the light amounts
	float atten = Attenuate(light, worldPos);
	float diff = saturate(dot(normal, toLight));

	// Calculate diffuse with energy conservation
	// (Reflected light doesn't diffuse)
	float3 balancedDiff = diff;

	// Combine
	return (balancedDiff * surfaceColor) * atten * light.intensity * light.color;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float3 normal = normalize(input.normal);

	// AmbientColor Calculation
	float3 ambientColor = float3(0, 0, 0);
	for (int a = 0; a < lights.ambientLightCount; ++a)
	{
		ambientColor += float3(lights.ambientLights[a].color.rgb * lights.ambientLights[a].intensity);
	}

	// Dir Light Calculation
	float3 dirColor = float3(0, 0, 0);
	for (int d = 0; d < lights.dirLightCount; ++d)
	{
		dirColor += CalcDirLight(lights.dirLights[d], normal);
	}

	// Point Light Calculation
	float3 pointColor = float3(0, 0, 0);
	for (int p = 0; p < lights.pointLightCount; ++p)
	{
		pointColor += CalcPointLight(lights.pointLights[p], normal, input.worldPos, cameraPos, surfaceColor.rgb);
	}

	float3 finalLightColor = ambientColor + dirColor + pointColor;
	float3 gamma = float3(pow(abs(finalLightColor.rgb * surfaceColor.rgb), (1.0 / 2.2)));

	return float4(gamma, 1);
}