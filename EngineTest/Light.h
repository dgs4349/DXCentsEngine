#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Color.h"

struct DirectionalLight
{
public:
	Color ambientColor;
	Color diffuseColor;
	DirectX::XMFLOAT3 direction;

	DirectionalLight();
	DirectionalLight(Color ambientColor, Color diffuseColor, DirectX::XMFLOAT3 direction);
};

