#pragma once

#include <vector>

#include "CentsEngine.h"
#include "Player.h"

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

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);

private:

	Lights lights;

	float startRopeSpeed = 100.0f;
	float ropeSpeed;
	float speedIncrease = 15.0f;
	float floorHeight = 0.0;
	float gravity = 6.5f;
	float jumpHeight = 4.0f;
	float ropeWidth = 10.0f;
	float ropeHeight = 0.5f;
	float timer;
	float readyLength = 1.0f;
	float endScreenLength = 1.0f;
	GameState gameState;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void LoadModels();
	void LoadTextures();
	void CreateMaterials();
	void CreateBasicGeometry();

	GameObject* rope;
	std::vector<Player*> players;
	GameObject* ground;

	std::vector<Mesh*> meshes;
	std::vector<GameObject*> gameObjects;
	std::vector<ID3D11ShaderResourceView*> textureViews;
	std::vector<Material*> materials;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Camera* camera;
	GameObject* camObject;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

