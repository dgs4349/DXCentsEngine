#pragma once
#include <DirectXMath.h>
#include "Transform.h"

struct DirectionalLight
{
public:
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 direction;

	DirectionalLight();
	DirectionalLight(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT3 direction);
};

