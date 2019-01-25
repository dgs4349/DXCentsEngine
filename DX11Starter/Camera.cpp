#include "Camera.h"

Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	DirectX::XMVECTOR move = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 movement = { 0.0f, 0.0f, 0.0f };

	if (GetAsyncKeyState('R') & 0x8000)
	{
		transform.Rotate(0.1f, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Forward()));
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Backward()));
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Left()));
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Right()));
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Up()));
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		move = DirectX::XMVectorAdd(move, DirectX::XMLoadFloat3(&transform.Down()));
	}

	move = DirectX::XMVectorAdd(
		DirectX::XMLoadFloat3(&transform.Position()),
		DirectX::XMVectorScale(move, deltaTime * 5.0f)
	);
	DirectX::XMStoreFloat3(&movement, move);
	transform.Position(movement);
}

DirectX::XMFLOAT4X4 Camera::ViewMatrix()
{
	UpdateViewMatrix();
	return viewMatrix;
}

void Camera::UpdateViewMatrix()
{
	transform.SetDirty();

	DirectX::XMStoreFloat4x4(&viewMatrix,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookToLH(
				DirectX::XMLoadFloat3(&transform.Position()),
				DirectX::XMLoadFloat3(&transform.Forward()),
				DirectX::XMLoadFloat3(&transform.Up())
			)
		)
	);
}
