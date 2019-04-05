#pragma once
#include "SimpleShader.h"
#include "Object.h"

class Material : public Object
{
public:
	Material();
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* diffuseTextureResourceView, ID3D11SamplerState* samplerState);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* diffuseTextureResourceView, ID3D11ShaderResourceView* normalTextureResourceView, ID3D11SamplerState* samplerState);

	SimpleVertexShader* VertexShader() const;
	SimplePixelShader* PixelShader() const;
	ID3D11ShaderResourceView* DiffuseTextureResourceView() const;
	ID3D11ShaderResourceView* NormalTextureResourceView() const;
	ID3D11SamplerState* SamplerState() const;
	DirectX::XMFLOAT2 uvOffset;
	bool HasNormalTexureResource() { return normalTextureResourceView != nullptr; }
	void SetMaterial() const;

protected:

	virtual ~Material() override;

private:
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	ID3D11ShaderResourceView* diffuseTextureResourceView = nullptr;
	ID3D11ShaderResourceView* normalTextureResourceView = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};

