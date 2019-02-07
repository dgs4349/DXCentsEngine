#pragma once
#include "SimpleShader.h"

class Material
{
public:
	Material();
	Material(SimpleVertexShader* const vertexShader, SimplePixelShader* const pixelShader);
	~Material();

	SimpleVertexShader* VertexShader() const;
	SimplePixelShader* PixelShader() const;

	void SetMaterial() const;

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};

