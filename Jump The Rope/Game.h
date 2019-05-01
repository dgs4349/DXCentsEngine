#pragma once

#include <vector>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include "CentsEngine.h"
#include "Player.h"
#include "CentsAudioHandler.h"
#include "Emitter.h"
#include "Flame.h"

namespace GameState
{
	enum GameState
	{
		PreStart,
		Playing,
		End
	};

#define GameState GameState::GameState
}

class Game
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void DrawSky();

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);

	void OnSuspending();
	void OnResuming();

private:

	Lights lights;

	CentsAudioHandler* audioHandler;
	std::vector<CentsSoundEffect*> jumpSfx;

	float startRopeSpeed = 100.0f;
	float ropeSpeed;
	float speedIncrease = 4.0f;
	float speedIncreaseMax = 500.0f;
	float floorHeight = 0.0;
	float gravity = 6.5f;
	float jumpHeight = 4.0f;
	float ropeWidth = 10.0f;
	float ropeHeight = 0.5f;
	float timer;
	float readyLength = 1.0f;
	float endScreenLength = 1.0f;
	float animSpeed = 0.2f; // the speed of the animation in seconds
	float animTimer = animSpeed; // the time it takes for the hash animation to move to the next frame
	int animFrame = 0; // the current frame of the animation
	GameState gameState;

	bool awardedJump = false;
	int numJumps = 0;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void LoadModels();
	void LoadTextures();
	void SetShaderHashTextures(float deltaTime);
	void CreateMaterials();
	void CreateBasicGeometry();

	void BlurRender(ID3D11RenderTargetView* RTV, ID3D11ShaderResourceView* SRV, float blurAmount, float dofAmount);


	GameObject* rope;
	std::vector<Player*> players;
	GameObject* ground;
	GameObject* tree1;
	GameObject* tree2;
	GameObject* tree3;
	GameObject* tree4;
	GameObject* tree5;
	GameObject* tree6;
	GameObject* tree7;
	GameObject* tree8;

	GameObject* cross;
	GameObject* grave1;
	GameObject* grave2;
	GameObject* grave3;
	GameObject* grave4;
	GameObject* grave5;
	GameObject* skel1;
	GameObject* skel2;
	GameObject* skel1Arm;
	GameObject* skel2Arm;

	GameObject* torch;

	GameObject* testParticle;

	CentsSoundEffect* bgIntro;
	CentsSoundEffect* bgLoop;

	GameObject* fog;

	Flame* flame1;

	ID3D11SamplerState* hashSampler;

	ID3D11ShaderResourceView* hashTexture1_1;
	ID3D11ShaderResourceView* hashTexture2_1;
	ID3D11ShaderResourceView* hashTexture1_2;
	ID3D11ShaderResourceView* hashTexture2_2;
	ID3D11ShaderResourceView* hashTexture1_3;
	ID3D11ShaderResourceView* hashTexture2_3;

	std::vector<Mesh*> meshes;
	std::vector<GameObject*> gameObjects;
	std::vector<ID3D11ShaderResourceView*> textureViews;
	std::vector<Material*> materials;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* particleVS;
	SimplePixelShader* particlePS;
	Camera* camera;
	GameObject* camObject;
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* particleDepthState;
	ID3D11BlendState* particleBlendState;
	
	ID3D11SamplerState* sampler;

	ID3D11ShaderResourceView* skySRV;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;
	ID3D11RasterizerState* skyRasterState;
	ID3D11DepthStencilState* skyDepthState;


	// Post process stuff -------------------
	ID3D11RenderTargetView* ppRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView* ppSRV;	// Allows us to sample from the same texture
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;

	ID3D11RenderTargetView* bloomRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView* bloomSRV;	// Allows us to sample from the same texture
	SimplePixelShader* bloomPS;

	ID3D11RenderTargetView* blurRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView* blurSRV;	// Allows us to sample from the same texture
	SimplePixelShader* GaussianHPS;
	SimplePixelShader* GaussianVPS;

	SimplePixelShader* DepthPS;

	ID3D11RenderTargetView* depthRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView* depthSRV;	// Allows us to sample from the same texture

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

