#include "Material.h"

Material::Material() : Object("Material")
{
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader) : Object("Material")
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* diffuseTextureResourceView, ID3D11SamplerState* samplerState) : Object("Material")
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->diffuseTextureResourceView = diffuseTextureResourceView;
	this->samplerState = samplerState;
}

Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader, ID3D11ShaderResourceView * diffuseTextureResourceView, ID3D11ShaderResourceView * normalTextureResourceView, ID3D11SamplerState * samplerState)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->diffuseTextureResourceView = diffuseTextureResourceView;
	this->normalTextureResourceView = normalTextureResourceView;
	this->samplerState = samplerState;
}

Material::~Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	diffuseTextureResourceView = nullptr;
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

ID3D11ShaderResourceView* Material::DiffuseTextureResourceView() const
{
	return diffuseTextureResourceView;
}

ID3D11ShaderResourceView * Material::NormalTextureResourceView() const
{
	return normalTextureResourceView;
}

ID3D11SamplerState * Material::SamplerState() const
{
	return samplerState;
}
