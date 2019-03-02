#include "Light.h"

DirectionalLight::DirectionalLight()
{
	ambientColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	diffuseColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

DirectionalLight::DirectionalLight(Color ambientColor, Color diffuseColor, DirectX::XMFLOAT3 direction)
{
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->direction = direction;
}