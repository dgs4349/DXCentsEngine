#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"

#include <iostream>

class Entity
{
public:
	Entity(Mesh* const mesh);
	~Entity();

	Mesh* const GetMesh() const;
	void SetMesh(Mesh* const mesh);

	DirectX::XMFLOAT3 Position() const;
	void Position(float x, float y, float z);
	void Position(DirectX::XMFLOAT3 position);

	DirectX::XMFLOAT3 Rotation() const;
	void Rotation(float x, float y, float z);
	void Rotation(DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 Scale() const;
	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT4X4 WorldMatrix();

private:

	bool dirty;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT4X4 worldMatrix;

	Mesh* mesh;

	void UpdateWorldMatrix();
};

