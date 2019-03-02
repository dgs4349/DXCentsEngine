#include "RenderManager.h"
#include "MeshRenderer.h"

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

void RenderManager::Render(Camera* camera) const
{
	XMFLOAT4X4 view = camera->ViewMatrix();
	XMFLOAT4X4 projection = camera->ProjectionMatrix();

	for (std::vector<Renderer*>::const_iterator it = activeRenderers.begin(); it != activeRenderers.end(); ++it)
	{
		const MeshRenderer* meshRenderer = reinterpret_cast<const MeshRenderer*>(*it);

		meshRenderer->PrepareMaterial(view, projection);
	}
}


RenderManager::~RenderManager()
{
}
