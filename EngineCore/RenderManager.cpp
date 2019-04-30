#include "RenderManager.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "Transform.h"

using namespace DirectX;

RenderManager::RenderManager()
{
}

void RenderManager::RegisterRenderer(Renderer * const renderer)
{
	LOG_TRACE("Adding Renderer to RenderManager");
	activeRenderers.push_back(renderer);
}

void RenderManager::UnregisterRenderer(Renderer * const renderer)
{
}

void RenderManager::UnregisterRenderer(uint64_t const rendererID)
{
}

void RenderManager::Render(Camera* camera, ID3D11DeviceContext* context, Lights& lights) const
{
	XMFLOAT4X4 view = camera->ViewMatrix();
	XMFLOAT4X4 projection = camera->ProjectionMatrix();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (std::vector<Renderer*>::const_iterator it = activeRenderers.begin(); it != activeRenderers.end(); ++it)
	{
		const MeshRenderer* meshRenderer = reinterpret_cast<const MeshRenderer*>(*it);
		const GameObject* gameObject = meshRenderer->gameObject;
		const Transform* transform = meshRenderer->transform;
		const MeshFilter* meshFilter = gameObject->GetComponent<MeshFilter>();

		meshRenderer->GetMaterial()->PixelShader()->SetData("lights", &lights, sizeof(Lights));
		meshRenderer->GetMaterial()->PixelShader()->SetData("cameraPos", &camera->transform->Position(), sizeof(XMFLOAT3));
		meshRenderer->GetMaterial()->PixelShader()->SetData("uvOffset", &meshRenderer->GetMaterial()->uvOffset, sizeof(XMFLOAT2));
		meshRenderer->PrepareMaterial(view, projection);

		context->IASetVertexBuffers(0, 1, meshFilter->GetMesh()->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(meshFilter->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(meshFilter->GetMesh()->GetIndexCount(), 0, 0);
	}
}

// this is just here so I can use my own shader for all this
void RenderManager::Render(Camera* camera, ID3D11DeviceContext* context, Lights& lights, SimplePixelShader * PS) const
{
	XMFLOAT4X4 view = camera->ViewMatrix();
	XMFLOAT4X4 projection = camera->ProjectionMatrix();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (std::vector<Renderer*>::const_iterator it = activeRenderers.begin(); it != activeRenderers.end(); ++it)
	{
		const MeshRenderer* meshRenderer = reinterpret_cast<const MeshRenderer*>(*it);
		const GameObject* gameObject = meshRenderer->gameObject;
		const Transform* transform = meshRenderer->transform;
		const MeshFilter* meshFilter = gameObject->GetComponent<MeshFilter>();

		meshRenderer->GetMaterial()->PixelShader()->SetData("lights", &lights, sizeof(Lights));
		meshRenderer->GetMaterial()->PixelShader()->SetData("cameraPos", &camera->transform->Position(), sizeof(XMFLOAT3));
		meshRenderer->GetMaterial()->PixelShader()->SetData("uvOffset", &meshRenderer->GetMaterial()->uvOffset, sizeof(XMFLOAT2));
		meshRenderer->PrepareMaterial(view, projection);

		PS->SetShader();

		context->IASetVertexBuffers(0, 1, meshFilter->GetMesh()->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(meshFilter->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(meshFilter->GetMesh()->GetIndexCount(), 0, 0);
	}
}

RenderManager::~RenderManager()
{
}
