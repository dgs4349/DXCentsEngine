#include "MeshRenderer.h"
#include "Transform.h"

using namespace DirectX;

MeshRenderer::MeshRenderer() : Renderer("Mesh Renderer")
{
	RenderManager::GetInstance()->RegisterRenderer(this);
}


MeshRenderer::MeshRenderer(Material* material) : Renderer("Mesh Renderer")
{
	RenderManager::GetInstance()->RegisterRenderer(this);
	this->material = material;
}

void MeshRenderer::RenderMesh()
{
	material->SetMaterial();
}

Material* const MeshRenderer::GetMaterial() const
{
	return material;
}


void MeshRenderer::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix) const
{
	material->PixelShader()->SetSamplerState("basicSampler", material->SamplerState());
	material->PixelShader()->SetShaderResourceView("diffuseTexture", material->ShaderResourceView());

	material->VertexShader()->SetMatrix4x4("view", viewMatrix);
	material->VertexShader()->SetMatrix4x4("projection", projectionMatrix);
	material->VertexShader()->SetMatrix4x4("world", transform->WorldMatrix());

	material->VertexShader()->SetShader();
	material->PixelShader()->SetShader();

	material->VertexShader()->CopyAllBufferData();
	material->PixelShader()->CopyAllBufferData();
}


MeshRenderer::~MeshRenderer()
{
}
