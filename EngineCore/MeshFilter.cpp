#include "MeshFilter.h"

MeshFilter::MeshFilter() : Component("Mesh Filter")
{
}

MeshFilter::MeshFilter(Mesh* mesh) : Component(mesh->GetMeshName().append(" (Mesh Filter)"))
{
	this->mesh = mesh;
}

const Mesh* MeshFilter::GetMesh() const
{
	return mesh;
}

void MeshFilter::SetMesh(Mesh * mesh)
{
	name = mesh->GetMeshName().append(" (Mesh Filter)");
	this->mesh = mesh;
}

MeshFilter::~MeshFilter()
{
}
