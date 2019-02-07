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

struct Light
{
public:

	Component::Transform transform;

	Light();
	~Light();

	void Color(DirectX::XMFLOAT4 color);
	DirectX::XMFLOAT4 Color() const;

private:

	DirectX::XMFLOAT4 color;

};

