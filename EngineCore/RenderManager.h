#pragma once
#include <DirectXMath.h>
#include "Singleton.h"
#include "Renderer.h"
#include "Camera.h"

class Renderer;

class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
	friend class Renderer;
public:
	RenderManager();

	void RegisterRenderer(Renderer* const renderer);
	void UnregisterRenderer(Renderer* const renderer);
	void UnregisterRenderer(uint64_t const rendererID);

	void Render(Camera* camera) const;

protected:

	std::vector<Renderer*> activeRenderers;

	~RenderManager();
};

