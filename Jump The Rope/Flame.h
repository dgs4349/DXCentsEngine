#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Emitter.h"
#include "Camera.h"


// This class holds several particle emitters and will organize the updating and drawing of each to produce a single flame effect
class Flame
{
public:
	Flame(DirectX::XMFLOAT3 position, ID3D11ShaderResourceView * texSmoke, ID3D11ShaderResourceView* texFire, ID3D11ShaderResourceView* texSparks,
		ID3D11Device* device, SimpleVertexShader* particleVS, SimplePixelShader* particlePS);
	~Flame();

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* context, Camera* camera);

private:
	Emitter* flame_smoke;
	Emitter* flame_add;
	Emitter* sparks;
	ID3D11BlendState* normBlendState;
	ID3D11BlendState* addBlendState;
	ID3D11DepthStencilState* partDepthState;
};

