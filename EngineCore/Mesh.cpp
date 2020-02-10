#include "Mesh.h"

#include <fstream>

using namespace DirectX;

Mesh::Mesh() : Object("Mesh")
{
}

Mesh::Mesh(const char* filePath, ID3D11Device* context) : Object("Mesh")
{
	LOG_TRACE("Loading mesh:\t\t\t{}", filePath);
	meshName = std::string(filePath);
	size_t fileName = meshName.find_last_of('/') + 1;
	size_t fileExtension = meshName.find('.');
	meshName = meshName.substr(fileName, fileExtension - fileName);

	// File input object
	std::ifstream obj(filePath);

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<XMFLOAT3> positions;     // Positions from the file
	std::vector<XMFLOAT3> normals;       // Normals from the file
	std::vector<XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<uint16_t> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

	// Still have data left?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.position = positions[i[0] - 1];
			v1.uv = uvs[i[1] - 1];
			v1.normal = normals[i[2] - 1];

			Vertex v2;
			v2.position = positions[i[3] - 1];
			v2.uv = uvs[i[4] - 1];
			v2.normal = normals[i[5] - 1];

			Vertex v3;
			v3.position = positions[i[6] - 1];
			v3.uv = uvs[i[7] - 1];
			v3.normal = normals[i[8] - 1];

			// The model is most likely in a right-handed space,
			// especially if it came from Maya.  We want to convert
			// to a left-handed space for DirectX.  This means we 
			// need to:
			//  - Invert the Z position
			//  - Invert the normal's Z
			//  - Flip the winding order
			// We also need to flip the UV coordinate since DirectX
			// defines (0,0) as the top left of the texture, and many
			// 3D modeling packages use the bottom left as (0,0)

			// Flip the UV's since they're probably "upside down"
			v1.uv.y = 1.f - v1.uv.y;
			v2.uv.y = 1.f - v2.uv.y;
			v3.uv.y = 1.f - v3.uv.y;

			// Flip Z (LH vs. RH)
			v1.position.z *= -1.f;
			v2.position.z *= -1.f;
			v3.position.z *= -1.f;

			// Flip normal Z
			v1.normal.z *= -1.f;
			v2.normal.z *= -1.f;
			v3.normal.z *= -1.f;

			// Add the verts to the vector (flipping the winding order)
			verts.push_back(v1);
			verts.push_back(v3);
			verts.push_back(v2);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.position = positions[i[9] - 1];
				v4.uv = uvs[i[10] - 1];
				v4.normal = normals[i[11] - 1];

				// Flip the UV, Z pos and normal
				v4.uv.y = 1.f - v4.uv.y;
				v4.position.z *= -1.f;
				v4.normal.z *= -1.f;

				// Add a whole triangle (flipping the winding order)
				verts.push_back(v1);
				verts.push_back(v4);
				verts.push_back(v3);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();

	CreateBuffers(&verts[0], static_cast<uint16_t>(verts.size()), &indices[0], static_cast<uint16_t>(indices.size()), context);
}

Mesh::~Mesh()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
	if (indexBuffer)
	{
		indexBuffer->Release();
	}
}

void Mesh::Clear()
{
	vertices.clear();
	triangles.clear();
}

std::string Mesh::GetMeshName() const
{
	return meshName;
}

std::vector<uint16_t> Mesh::GetTriangles() const
{
	return triangles;
}

std::vector<Vertex> Mesh::GetVertices() const
{
	return vertices;
}

uint16_t Mesh::GetVertexCount() const
{
	return vertexCount;
}

uint32_t Mesh::GetIndexCount() const
{
	return indexCount;
}

ID3D11Buffer* const* Mesh::GetVertexBuffer() const
{
	return &vertexBuffer;
}

ID3D11Buffer* const Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}

void Mesh::CreateBuffers(Vertex* const vertices, uint16_t vertexCount, uint16_t* const indices, uint16_t indexCount, ID3D11Device* const context)
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

	context->CreateBuffer(&VBD, &vertexData, &vertexBuffer);
	context->CreateBuffer(&IBD, &indexData, &indexBuffer);
}