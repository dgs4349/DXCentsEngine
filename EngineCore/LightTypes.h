#ifndef LIGHTS_H
#define LIGHTS_H

#define MAX_LIGHTS 128

#if defined(__cplusplus)

#include <DirectXMath.h>
#include "Color.h"

#define COLOR Color
#define FLOAT3 DirectX::XMFLOAT3

#else

#define COLOR float4
#define FLOAT3 float3

#endif

/// <summary>
/// Light that lights every object from no specific direction
/// </summary>
struct AmbientLight
{
	COLOR color;
	float intensity;
};

/// <summary>
/// Light emitted to all object equally in a direction
/// </summary>
struct DirectionalLight
{
	COLOR color;

	FLOAT3 direction;
	float intensity;
};

/// <summary>
/// Light emitted from a single point in space
/// </summary>
struct PointLight
{
	FLOAT3 color;
	float range;

	FLOAT3 position;
	float intensity;
};

/// <summary>
/// Light emitted in a direction from it's position
/// </summary>
struct SpotLight
{
	COLOR color;

	FLOAT3 position;
	float range;

	FLOAT3 direction;
	float angle;
};

#endif