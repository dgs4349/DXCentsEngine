#include "Material.h"

Material::Material() : Object("Material")
{
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader) : Object("Material")
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->uvOffset = DirectX::XMFLOAT2(0, 0);
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResourceView, ID3D11SamplerState* samplerState) : Object("Material")
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->shaderResourceView = shaderResourceView;
	this->samplerState = samplerState;
	this->uvOffset = DirectX::XMFLOAT2(0, 0);
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
