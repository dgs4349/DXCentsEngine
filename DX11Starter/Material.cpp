#include "Material.h"

Material::Material()
{
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResourceView, ID3D11SamplerState* samplerState)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->shaderResourceView = shaderResourceView;
	this->samplerState = samplerState;
}

Material::~Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	shaderResourceView = nullptr;
	samplerState->Release();
}

void Material::SetMaterial() const
{
	vertexShader->SetShader();
	pixelShader->SetShader();
}

SimpleVertexShader* Material::VertexShader() const
{
	return vertexShader;
}

SimplePixelShader* Material::PixelShader() const
{
	return pixelShader;
}

ID3D11ShaderResourceView* Material::ShaderResourceView() const
{
	return shaderResourceView;
}

ID3D11SamplerState * Material::SamplerState() const
{
	return samplerState;
}
