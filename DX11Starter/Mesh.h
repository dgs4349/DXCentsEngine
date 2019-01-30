#pragma once
#include <d3d11.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* const vertices, uint16_t vertexCount, uint16_t* const indices, uint16_t indexCount, ID3D11Device* context);
	Mesh(char* const filePath, ID3D11Device* context);
	~Mesh();

	ID3D11Buffer* const* GetVertexBuffer() const;
	ID3D11Buffer* const GetIndexBuffer() const;
	uint16_t GetIndexCount() const;

private:
	ID3D11Buffer* VBO = nullptr;
	ID3D11Buffer* VBI = nullptr;

	uint16_t indexCount = 0;

	void CreateBuffers(Vertex* const vertices, uint16_t vertexCount, uint16_t* const indices, uint16_t indexCount, ID3D11Device* const context);
};