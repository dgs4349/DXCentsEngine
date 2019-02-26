#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string.h>
#include "Object.h"
#include "Vertex.h"

class Mesh : public Object
{
public:
	Mesh();
	Mesh(const char* filePath, ID3D11Device* DXDevice);

	void Clear();
	std::string GetMeshName() const;
	std::vector<uint16_t> GetTriangles() const;
	std::vector<Vertex> GetVertices() const;
	uint16_t GetVertexCount() const;
	uint32_t GetIndexCount() const;

	ID3D11Buffer* const* GetVertexBuffer() const;
	ID3D11Buffer* const GetIndexBuffer() const;

protected:

	virtual ~Mesh() override;

private:

	std::string meshName;
	using Object::name;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	uint16_t vertexCount;
	uint32_t indexCount;

	std::vector<Vertex> vertices;
	std::vector<uint16_t> triangles;

	void CreateBuffers(Vertex* const vertices, uint16_t vertexCount, uint16_t* const indices, uint16_t indexCount, ID3D11Device* const context);
};

