#include "Light.h"

DirectionalLight::DirectionalLight()
{
	ambientColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuseColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

DirectionalLight::DirectionalLight(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT3 direction)
{
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->direction = direction;
}