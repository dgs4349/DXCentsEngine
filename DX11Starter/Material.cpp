#include "Material.h"

Material::Material()
{
}

Material::Material(SimpleVertexShader * const vertexShader, SimplePixelShader * const pixelShader)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

Material::~Material()
{
	delete vertexShader;
	delete pixelShader;
}

void Material::SetMaterial() const
{
	vertexShader->SetShader();
	pixelShader->SetShader();
}

SimpleVertexShader* const Material::VertexShader() const
{
	return vertexShader;
}

SimplePixelShader* const Material::PixelShader() const
{
	return pixelShader;
}
