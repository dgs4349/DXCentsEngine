#include "Flame.h"

using namespace DirectX;

Flame::Flame(DirectX::XMFLOAT3 position, ID3D11ShaderResourceView * texSmoke, ID3D11ShaderResourceView * texFire, ID3D11ShaderResourceView * texSparks, ID3D11Device * device,
	SimpleVertexShader * particleVS, SimplePixelShader * particlePS)
{
	flame_smoke = new Emitter(
		200,								// Max particles
		20,									// Particles per second
		7,									// Particle lifetime
		0.3f,								// Start size
		0.1f,								// End size
		XMFLOAT4(0.35f, 0.05f, 0.02f, 0.7f),// Start color
		XMFLOAT4(0.35f, 0.05f, 0.02f, 0),	// End color
		XMFLOAT3(0, .3f, 0),				// Start velocity
		XMFLOAT3(0.05f, 0.2f, 0.05f),		// Velocity randomness range
		position,							// Emitter position
		XMFLOAT3(0.1f, 0, 0.1f),			// Position randomness range
		XMFLOAT4(-2, 2, -2, 2),				// Random rotation ranges (startMin, startMax, endMin, endMax)
		XMFLOAT3(0, -0.01f, 0),				// Constant acceleration
		device,
		particleVS,
		particlePS,
		texSmoke);

	flame_add = new Emitter(
		100,								// Max particles
		15,									// Particles per second
		2,									// Particle lifetime
		0.3f,								// Start size
		0.1f,								// End size
		XMFLOAT4(1, 0.78f, 0.12f, 0.7f),	// Start color
		XMFLOAT4(0.9f, 0.14f, 0.02f, 0),	// End color
		XMFLOAT3(0, .8f, 0),				// Start velocity
		XMFLOAT3(0.07f, 0.1f, 0.07f),		// Velocity randomness range
		position,							// Emitter position
		XMFLOAT3(0.2f, 0, 0.2f),			// Position randomness range
		XMFLOAT4(-2, 2, -2, 2),				// Random rotation ranges (startMin, startMax, endMin, endMax)
		XMFLOAT3(0, -0.01f, 0),				// Constant acceleration
		device,
		particleVS,
		particlePS,
		texFire);

	sparks = new Emitter(
		200,							// Max particles
		15,								// Particles per second
		7,								// Particle lifetime
		0.05f,							// Start size
		0.05f,							// End size
		XMFLOAT4(1, 0.1f, 0.1f, 0.7f),	// Start color
		XMFLOAT4(1, 0.6f, 0.1f, 0),		// End color
		XMFLOAT3(0, .3f, 0),			// Start velocity
		XMFLOAT3(0.01f, 0.1f, 0.01f),	// Velocity randomness range
		position,						// Emitter position
		XMFLOAT3(0.2f, 0, 0.2f),		// Position randomness range
		XMFLOAT4(-2, 2, -2, 2),			// Random rotation ranges (startMin, startMax, endMin, endMax)
		XMFLOAT3(0, -0.01f, 0),			// Constant acceleration
		device,
		particleVS,
		particlePS,
		texSparks);
}

Flame::~Flame()
{
	delete flame_smoke;
	delete flame_add;
	delete sparks;
}

void Flame::Update(float deltaTime)
{
	flame_smoke->Update(deltaTime);
	flame_add->Update(deltaTime);
	sparks->Update(deltaTime);
}

void Flame::Draw(ID3D11DeviceContext * context, Camera * camera)
{
	flame_smoke->Draw(context, camera);
	flame_add->Draw(context, camera);
	sparks->Draw(context, camera);
}
