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
Texture2D hashTexture1 : register(t2);
Texture2D hashTexture2 : register(t3);

float Attenuate(float3 position, float range, float3 worldPos)
{
	float dist = distance(position, worldPos);

	// Ranged-based attenuation
	float att = saturate(1.0f - (dist * dist / (range * range)));

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
	float atten = Attenuate(light.position, light.range, worldPos);
	float diff = saturate(dot(normal, toLight));

	// Calculate diffuse with energy conservation
	// (Reflected light doesn't diffuse)
	float3 balancedDiff = diff;

	// Combine
	return (balancedDiff * surfaceColor) * atten * light.intensity * light.color;
}

float3 CalcSpotLight(SpotLight light, float3 normal, float3 worldPos, float3 camPos, float3 surfaceColor)
{
	float3 toLight = normalize(light.position - worldPos);
	float3 toCam = normalize(camPos - worldPos);

	float centerAngle = max(dot(toLight, light.direction), 0.0f);
	float spotAmount = pow(centerAngle, light.angle);

	float atten = Attenuate(light.position, light.range, worldPos);
	float diff = saturate(dot(normal, toLight));

	float3 balancedDiff = diff;

	return (balancedDiff * surfaceColor) * spotAmount * light.intensity * light.color;
}

float3 hashing(float2 uv, float intensity) {
	float3 hash1 = hashTexture1.Sample(basicSampler, uv).rgb;
	float3 hash2 = hashTexture2.Sample(basicSampler, uv).rgb;

	float3 overbright = min(0, intensity); //how much over the limit of 1 the intensity is. Will have much less hashing

	//each hash texture represents an intensity of exactly a multiple of 1/6
	//handles weighting between hash textures if intensity != multiple of 1/6
	float3 weights1 = saturate((intensity * 6.0f) + float3(0, -1.0f, -2.0f));
	float3 weights2 = saturate((intensity * 6.0f) + float3(-3.0f, -4.0f, -5.0f));

	weights1.xy -= weights1.yz;
	weights1.z -= weights2.x;
	weights2.xy -= weights2.zy;

	hash1 = hash1 * weights1;
	hash2 = hash2 * weights2;

	float3 hashing = overbright + hash1.r +
		hash1.g + hash1.b +
		hash2.r + hash2.g +
		hash2.b;

	return hashing;
}

float4 main(VertexToPixel input) : SV_TARGET
{


	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv + uvOffset);
	float3 normalFromMap = normalTexture.Sample(basicSampler, input.uv).rgb * 2 - 1;

	// Calculate the matrix we'll use to convert from tangent to world space
	float3 N = normalize(input.normal);
	float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	// Use the normal from the map, after we've converted it to world space
	float3 normal = normalize(mul(normalFromMap, TBN));


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

	// Spot Light Calculation
	float3 spotColor = float3(0, 0, 0);
	for (int s = 0; s < lights.spotLightCount; ++s)
	{
		spotColor += CalcSpotLight(lights.spotLights[s], normal, input.worldPos, cameraPos, surfaceColor.rgb);
	}

	float3 finalLightColor = ambientColor + dirColor + pointColor + spotColor;

	// hashmark shading code

	// determines intensity of light at pixel
	float intensity = dot(finalLightColor, float3(0.2326, 0.7152, 0.0722)); //constant vector from the luminosity function

	intensity = 1.0 - intensity;
	
	//return float4(intensity, intensity, intensity, 1);
	return float4(hashing(input.uv * 16.0f, intensity), 1.0f);

	float3 gamma = float3(pow(abs(finalLightColor.rgb * surfaceColor.rgb), (1.0 / 2.2)));
	
	return float4(gamma, surfaceColor.a);
}