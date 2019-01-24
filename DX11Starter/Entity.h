#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"

class Entity
{
public:

	Component::Transform transform;

	Entity(Mesh* const mesh);
	~Entity();

	Mesh* const GetMesh() const;
	void SetMesh(Mesh* const mesh);

private:
	Mesh* mesh;
};

