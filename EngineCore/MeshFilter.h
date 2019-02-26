#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshFilter : Component
{
public:
	MeshFilter();
	MeshFilter(Mesh* mesh);

	const Mesh* GetMesh() const;
	void SetMesh(Mesh* mesh);

protected:

	Mesh* mesh = nullptr;

	virtual ~MeshFilter() override;
};

