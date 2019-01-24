#include "Camera.h"

Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
}


Camera::~Camera()
{
}

void Camera::Update()
{
	DirectX::XMStoreFloat4x4(&viewMatrix,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtLH(
				DirectX::XMLoadFloat3(&transform.Position()),
				DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&transform.Forward()), DirectX::XMLoadFloat3(&transform.Position())),
				DirectX::XMLoadFloat3(&transform.Up())
			)
		)
	);
}


DirectX::XMFLOAT4X4 Camera::ViewMatrix() const
{
	return viewMatrix;
}
