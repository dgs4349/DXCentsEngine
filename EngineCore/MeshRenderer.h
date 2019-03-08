#pragma once
#include <DirectXMath.h>
#include "Renderer.h"
#include "Material.h"

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	MeshRenderer(Material* material);

	void RenderMesh();
	Material* const GetMaterial() const;
	void SetMaterial(Material* const material);
	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix) const;

protected:

	virtual ~MeshRenderer() override;
};

