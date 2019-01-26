#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

class Entity
{
public:

	Component::Transform transform;
	Material* material;

	Entity(Mesh* const mesh, Material* const material);
	~Entity();

	Mesh* const GetMesh() const;
	void SetMesh(Mesh* const mesh);

	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, const DirectX::XMFLOAT4X4 projectionMatrix);

private:
	Mesh* mesh;
};

