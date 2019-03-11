#ifndef LIGHTS_H
#define LIGHTS_H

#define MAX_LIGHTS 8

#if defined(__cplusplus)

#include <DirectXMath.h>
#include "Color.h"

#define COLOR3 Color3
#define COLOR4 Color
#define FLOAT3 DirectX::XMFLOAT3
#define INTENSITY float intensity = 1
#else

#define COLOR3 float3
#define COLOR4 float4
#define FLOAT3 float3
#define INTENSITY float intensity

#endif

/// <summary>
/// Light that lights every object from no specific direction
/// </summary>
struct AmbientLight
{
	COLOR3 color;
	INTENSITY;
};


/// <summary>
/// Light emitted to all object equally in a direction
/// </summary>
struct DirectionalLight
{
	COLOR4 color;

	FLOAT3 direction;
	INTENSITY;
};


/// <summary>
/// Light emitted from a single point in space
/// </summary>
struct PointLight
{
	FLOAT3 color;
	float range;

	FLOAT3 position;
	INTENSITY;
};


/// <summary>
/// Light emitted in a direction from it's position
/// </summary>
struct SpotLight
{
	COLOR3 color;
	INTENSITY;

	FLOAT3 position;
	float range;

	FLOAT3 direction;
	float angle;
};


struct Lights
{
	AmbientLight ambientLights[MAX_LIGHTS];
	DirectionalLight dirLights[MAX_LIGHTS];
	PointLight pointLights[MAX_LIGHTS];
	SpotLight spotLights[MAX_LIGHTS];
	int ambientLightCount;
	int dirLightCount;
	int pointLightCount;
	int spotLightCount;
};

#endif