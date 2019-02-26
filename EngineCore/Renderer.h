#pragma once
#include <string>
#include "Component.h"
#include "Material.h"
#include "RenderManager.h"

class Renderer : public Component
{
	friend class RenderManager;
public:
	Renderer();
	Renderer(std::string name);

protected:

	Material* material = nullptr;

	virtual ~Renderer() override;
};

