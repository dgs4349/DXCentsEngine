#include "RenderManager.h"



RenderManager::RenderManager()
{
}

void RenderManager::RegisterRenderer(Renderer * const renderer)
{
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
	for (int i = 0; i < activeRenderers.size(); ++i)
	{

	}
}


RenderManager::~RenderManager()
{
}
