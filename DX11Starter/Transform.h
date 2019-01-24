#pragma once

#include <iostream>
#include <DirectXMath.h>

namespace Component
{
	struct Transform
	{
	public:

		Transform();

		DirectX::XMFLOAT3 Position() const;
		void Position(float x, float y, float z);
		void Position(DirectX::XMFLOAT3 position);

		DirectX::XMFLOAT3 Rotation() const;
		void Rotation(float x, float y, float z);
		void Rotation(DirectX::XMFLOAT3 rotation);

		DirectX::XMFLOAT3 Scale() const;
		void Scale(float x, float y, float z);
		void Scale(DirectX::XMFLOAT3 scale);

		DirectX::XMFLOAT3 Forward();
		DirectX::XMFLOAT3 Up();
		DirectX::XMFLOAT3 Left();

		/// Get the world matrix of this Transform.
		DirectX::XMFLOAT4X4 WorldMatrix();

	private:

		bool dirty;

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;

		DirectX::XMFLOAT3 forward;
		DirectX::XMFLOAT3 up;
		DirectX::XMFLOAT3 left;

		DirectX::XMFLOAT4X4 worldMatrix;

		void UpdateTransform();
		void UpdateWorldMatrix();
		void UpdateDirectionalVectors();
	};
}