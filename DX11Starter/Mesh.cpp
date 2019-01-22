#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(Vertex* const vertices, uint16_t vertexCount, uint16_t* const indices, uint16_t indexCount, ID3D11Device* context)
{
	this->indexCount = indexCount;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;

	D3D11_BUFFER_DESC VBD;
	VBD.Usage = D3D11_USAGE_IMMUTABLE;
	VBD.ByteWidth = sizeof(Vertex) * vertexCount;
	VBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBD.CPUAccessFlags = 0;
	VBD.MiscFlags = 0;
	VBD.StructureByteStride = 0;

	D3D11_BUFFER_DESC IBD;
	IBD.Usage = D3D11_USAGE_IMMUTABLE;
	IBD.ByteWidth = sizeof(uint16_t) * indexCount;
	IBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBD.CPUAccessFlags = 0;
	IBD.MiscFlags = 0;
	IBD.StructureByteStride = 0;

	context->CreateBuffer(&VBD, &vertexData, &VBO);
	context->CreateBuffer(&IBD, &indexData, &VBI);
}

ID3D11Buffer* const* Mesh::GetVertexBuffer() const
{
	return &VBO;
}

ID3D11Buffer* const Mesh::GetIndexBuffer() const
{
	return VBI;
}

uint16_t Mesh::GetIndexCount() const
{
	return indexCount;
}

Mesh::~Mesh()
{
	if (VBO != nullptr)
	{
		VBO->Release();
		VBO = nullptr;
	}
	if (VBI != nullptr)
	{
		VBI->Release();
		VBI = nullptr;
	}
}