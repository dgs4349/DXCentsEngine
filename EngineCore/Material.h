#pragma once
#include "SimpleShader.h"
#include "Object.h"

class Material : public Object
{
public:
	Material();
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResourceView, ID3D11SamplerState* samplerState);

	SimpleVertexShader* VertexShader() const;
	SimplePixelShader* PixelShader() const;
	ID3D11ShaderResourceView* ShaderResourceView() const;
	ID3D11SamplerState* SamplerState() const;

	void SetMaterial() const;

protected:

	virtual ~Material() override;

private:
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};

