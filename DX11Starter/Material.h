#pragma once
#include "SimpleShader.h"

class Material
{
public:
	Material();
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResourceView, ID3D11SamplerState* samplerState);
	~Material();

	SimpleVertexShader* VertexShader() const;
	SimplePixelShader* PixelShader() const;
	ID3D11ShaderResourceView* ShaderResourceView() const;
	ID3D11SamplerState* SamplerState() const;

	void SetMaterial() const;

private:
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};

