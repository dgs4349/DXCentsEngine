#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

/*
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
*/

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance) : DXCore(hInstance, const_cast<char*>("DirectX Game"), 1280, 720, true)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

	Logger::GetInstance();
	ObjectManager::GetInstance();
	RenderManager::GetInstance();

	camObject = new GameObject("Camera");

	camera = camObject->AddComponent<Camera>();
	lights = Lights();

	audioHandler = new CentsAudioHandler();
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	ObjectManager::ReleaseInstance();
	RenderManager::ReleaseInstance();

	delete vertexShader;
	delete pixelShader;

	for (int i = 0; i < textureViews.size(); ++i)
	{
		textureViews[i]->Release();
	}

	textureViews.clear();
	materials.clear();
	gameObjects.clear();
	meshes.clear();

	delete audioHandler;

	Logger::ReleaseInstance();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	LoadTextures();
	CreateMaterials();
	LoadModels();
	CreateBasicGeometry();

	camera->transform->Position(7.0f, 2.0f, -13.0f);
	camera->transform->Rotate(0.0f, -30.0f, 0.0f);
	camera->SetScreenSize(width, height);

	lights.ambientLights[0] = { Color(0.2f), 1 };
	lights.ambientLightCount = 1;

	lights.dirLights[0] = { Color(1, 0, 0, 1), XMFLOAT3(1, 0, 0) };
	lights.dirLightCount = 1;

	lights.pointLights[0] = { XMFLOAT3(1, 1, 1), 2, XMFLOAT3(3.5f, 0.5f, 0), 10 };
	lights.pointLightCount = 1;
	
	lights.spotLights[0] = { XMFLOAT3(0, 0, 1), 10, XMFLOAT3(0, 1, 0), 10, XMFLOAT3(-1, 0, 0), 10 };
	lights.spotLightCount = 1;

	/*ambientLight = { Color(0.5f) };
	directionalLight = { Color(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	directionalLight2 = { Color(0.0f, 0.0f, 0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };*/

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	audioHandler->Init();
	// loading audio here for now
	bgIntro = audioHandler->CreateSoundEffect(L"Assets/Audio/audio_background_intro.wav");
	bgLoop = audioHandler->CreateSoundEffect(L"Assets/Audio/audio_background_loop.wav", true);
	bgIntro->Link(bgLoop);
	bgIntro->Set(-0.5f); // not working
	bgIntro->PlayOnUpdate();
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

void Game::LoadModels()
{
	meshes.push_back(new Mesh("Assets/Models/cone.obj", device));
	meshes.push_back(new Mesh("Assets/Models/cube.obj", device));
	meshes.push_back(new Mesh("Assets/Models/cylinder.obj", device));
	meshes.push_back(new Mesh("Assets/Models/helix.obj", device));
	meshes.push_back(new Mesh("Assets/Models/sphere.obj", device));
	meshes.push_back(new Mesh("Assets/Models/torus.obj", device));
	meshes.push_back(new Mesh("Assets/Models/rope.obj", device));
	meshes.push_back(new Mesh("Assets/Models/skeleboy_body.obj", device));
	meshes.push_back(new Mesh("Assets/Models/skeleboy_arm1.obj", device));
	meshes.push_back(new Mesh("Assets/Models/ghostulon.obj", device));
	meshes.push_back(new Mesh("Assets/Models/Ground.obj", device));
	meshes.push_back(new Mesh("Assets/Models/Tree.obj", device));
	meshes.push_back(new Mesh("Assets/Models/Cross.obj", device));
	meshes.push_back(new Mesh("Assets/Models/Grave.obj", device));
}

void Game::LoadTextures()
{
	ID3D11ShaderResourceView* texView1;
	ID3D11ShaderResourceView* texView2;
	ID3D11ShaderResourceView* texView3;
	ID3D11ShaderResourceView* texView4;
	ID3D11ShaderResourceView* texView5;
	ID3D11ShaderResourceView* texView6;
	ID3D11ShaderResourceView* texView7;
	ID3D11ShaderResourceView* texView8;
	ID3D11ShaderResourceView* texView9;
	ID3D11ShaderResourceView* texView10;
	ID3D11ShaderResourceView* texView11;

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Cobblestone.jpg", 0, &texView1);
	textureViews.push_back(texView1);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Dirt.jpg", 0, &texView2);
	textureViews.push_back(texView2);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/StonePath.jpg", 0, &texView3);
	textureViews.push_back(texView3);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/WoodCrate.jpg", 0, &texView4);
	textureViews.push_back(texView4);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/skelebonesTex.png", 0, &texView5);
	textureViews.push_back(texView5);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/ghostoTex1.png", 0, &texView6);
	textureViews.push_back(texView6);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/ghostoTex2.png", 0, &texView7);
	textureViews.push_back(texView7);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/ghostoTex3.png", 0, &texView8);
	textureViews.push_back(texView8);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/ghostoTex4.png", 0, &texView9);
	textureViews.push_back(texView9);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/ghostoTex5.png", 0, &texView10);
	textureViews.push_back(texView10);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Ground.png", 0, &texView11);
	textureViews.push_back(texView11);

	ID3D11ShaderResourceView * testNormalMap;
	CreateWICTextureFromFile(device, context, L"Assets/Textures/normal.tif", 0, &testNormalMap);
	textureViews.push_back(testNormalMap);
}

void Game::CreateMaterials()
{
	ID3D11SamplerState* samplerState;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[0], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[1], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[2], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[3], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[4], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[5], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[6], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[7], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[8], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);

	materials.push_back(new Material(vertexShader, pixelShader, textureViews[9], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[10], textureViews[11], samplerState));
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Rope
	rope = new GameObject("Rope", meshes[6], materials[3]);

	// Left Player
	GameObject* player1 = new GameObject("Player 1", meshes[9], materials[5]);
	players.push_back(player1->AddComponent<Player>());
	// Right Player
	GameObject* player2 = new GameObject("Player 2", meshes[9], materials[6]);
	players.push_back(player2->AddComponent<Player>());

	// Ground
	ground = new GameObject("Ground", meshes[10], materials[10]);

	// Props
	tree1 = new GameObject("Tree1", meshes[11], materials[1]);
	tree2 = new GameObject("Tree2", meshes[11], materials[1]);
	tree3 = new GameObject("Tree3", meshes[11], materials[1]);
	tree4 = new GameObject("Tree4", meshes[11], materials[1]);
	tree5 = new GameObject("Tree5", meshes[11], materials[1]);
	tree6 = new GameObject("Tree3", meshes[11], materials[1]);
	tree7 = new GameObject("Tree4", meshes[11], materials[1]);
	tree8 = new GameObject("Tree5", meshes[11], materials[1]);
	cross = new GameObject("Cross", meshes[12], materials[0]);
	grave1 = new GameObject("Grave1", meshes[13], materials[0]);
	grave2 = new GameObject("Grave2", meshes[13], materials[0]);
	grave3 = new GameObject("Grave3", meshes[13], materials[0]);
	grave4 = new GameObject("Grave4", meshes[13], materials[0]);
	grave5 = new GameObject("Grave5", meshes[13], materials[0]);

	// Skeletons
	skel1 = new GameObject("Skeleton1", meshes[7], materials[4]);
	skel2 = new GameObject("Skeleton2", meshes[7], materials[4]);
	skel1Arm = new GameObject("Skeleton1Arm", meshes[8], materials[4]);
	skel2Arm = new GameObject("Skeleton2Arm", meshes[8], materials[4]);

	// Left Player
	player1->transform->Position(-1.0f, 0.0f, 0.0f);
	player1->transform->Scale(1.0f, 1.0f, 1.0f);

	// Right Player
	player2->transform->Position(1.0f, 0.0f, 0.0f);
	player2->transform->Scale(1.0f, 1.0f, 1.0f);

	// Rope
	rope->transform->Position(0.0f, 0.5f, 0.0f);
	rope->transform->Scale(2.0f, 2.0f, 2.0f);
	rope->transform->EulerRotation(0, 0, 0);
	// Ground
	ground->transform->Position(0.0f, -1.0f, 0.0f);
	ground->transform->Scale(1.0f, 1.0f, 1.0f);

	// Props
	tree1->transform->Position(-11.67f, -1.0f, 12.8f);
	tree1->transform->Scale(1.0f, 1.0f, 1.0f);

	tree2->transform->Position(-14.14f, -1.0f, 5.9f);
	tree2->transform->Scale(1.0f, 1.0f, 1.0f);

	tree3->transform->Position(-13.47f, -1.0f, -2.5f);
	tree3->transform->Scale(1.0f, 1.0f, 1.0f);

	tree4->transform->Position(-4.2f, -1.0f, 13.0f);
	tree4->transform->Scale(1.0f, 1.0f, 1.0f);

	tree5->transform->Position(-9.5f, -1.0f, 20.76f);
	tree5->transform->Scale(1.0f, 1.0f, 1.0f);

	tree6->transform->Position(-20.5f, -1.0f, -9.4f);
	tree6->transform->Scale(1.0f, 1.0f, 1.0f);

	tree7->transform->Position(-12.82f, -1.0f, -13.430f);
	tree7->transform->Scale(1.0f, 1.0f, 1.0f);

	tree8->transform->Position(5.0f, -1.0f, 15.0f);
	tree8->transform->Scale(1.0f, 1.0f, 1.0f);

	cross->transform->Position(-10.54f, -1.0f, 9.48f);
	cross->transform->Scale(1.0f, 1.0f, 1.0f);
	cross->transform->Rotate(0.0, -24.5, 0.0);

	grave1->transform->Position(-7.0f, -1.0f, 10.50f);
	grave1->transform->Scale(1.0f, 1.0f, 1.0f);
	grave1->transform->Rotate(0.0, -5.0, 0.0);

	grave2->transform->Position(-3.0f, -1.0f, 10.50f);
	grave2->transform->Scale(1.0f, 1.0f, 1.0f);
	grave2->transform->Rotate(0.0, 5.0, 0.0);

	grave3->transform->Position(1.0f, -1.0f, 10.50f);
	grave3->transform->Scale(1.0f, 1.0f, 1.0f);
	grave3->transform->Rotate(0.0, 5.0, 0.0);

	grave4->transform->Position(-10.0f, -1.0f, 4.00f);
	grave4->transform->Scale(1.0f, 1.0f, 1.0f);
	grave4->transform->Rotate(0.0, -95.0, 0.0);

	grave5->transform->Position(-10.0f, -1.0f, 0.00f);
	grave5->transform->Scale(1.0f, 1.0f, 1.0f);
	grave5->transform->Rotate(0.0, -85.0, 0.0);

	// Skeletons
	skel1->transform->Position(5.6f, -1.0f, 0.0f);
	skel1->transform->EulerRotation(0, 90.0f, 0);

	skel2->transform->Position(-5.6f, -1.0f, 0.0f);
	skel2->transform->EulerRotation(0, -90.0f, 0);

	skel1Arm->transform->Position(5.6f, 0.54f, 0.0f);
	skel1Arm->transform->EulerRotation(0, 90.0f, 0);

	skel2Arm->transform->Position(-5.6f, 0.54f, 0.0f);
	skel2Arm->transform->EulerRotation(0, -90.0f, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	camera->OnResize();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	bool p1Input = GetAsyncKeyState(*"Q");
	bool p2Input = GetAsyncKeyState(*"P");

	if (gameState == GameState::PreStart)
	{
		if (p1Input)
		{
			players[0]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[8]);
		}
		else
		{
			players[0]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[5]);
		}

		if (p2Input)
		{
			players[1]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[9]);
		}
		else
		{
			players[1]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[6]);
		}

		if (p1Input && p2Input)
		{
			timer += deltaTime;

			if (timer >= readyLength)
			{
				gameState = GameState::Playing;

				timer = 0;
				ropeSpeed = startRopeSpeed;

				players[0]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[5]);
				players[1]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[6]);
			}
		}
		else
		{
			timer = 0;
		}
	}
	if (gameState == GameState::Playing)
	{
		ropeSpeed += speedIncrease * deltaTime;

		if (p1Input)
		{
			players[0]->Jump(jumpHeight);
		}
		if (p2Input)
		{
			players[1]->Jump(jumpHeight);
		}

		if (rope->transform->EulerAngles().x > 180 - ropeWidth && rope->transform->EulerAngles().x < 180 + ropeWidth)
		{
			for (int i = 0; i < players.size(); ++i)
			{
				if (players[i]->transform->Position().y < ropeHeight)
				{
					players[i]->gameObject->GetComponent<MeshRenderer>()->SetMaterial(materials[7]);
					gameState = GameState::End;
				}
			}
		}
	}
	if (gameState == GameState::End)
	{

		timer += deltaTime;

		if (timer > endScreenLength && rope->transform->EulerAngles().x == 0)
		{
			gameState = GameState::PreStart;
			timer = 0;
		}

		if (rope->transform->EulerAngles().x < 5 || rope->transform->EulerAngles().x > 355)
		{
			rope->transform->EulerRotation(0, 0, 0);
			ropeSpeed = 0;
		}
	}

	//rotates rope and arms
	rope->transform->Rotate({ ropeSpeed * deltaTime, 0, 0 });
	skel1Arm->transform->Rotate({ 0, 0, ropeSpeed * deltaTime});
	skel2Arm->transform->Rotate({ 0, 0, -ropeSpeed * deltaTime});

	for (int i = 0; i < players.size(); ++i)
	{
		players[i]->accelleration -= gravity * deltaTime;
		players[i]->Update(deltaTime);

		XMFLOAT3 pos = players[i]->transform->Position();

		if (pos.y < floorHeight)
		{
			pos.y = floorHeight;
			players[i]->transform->Position(pos);
			players[i]->onGround = true;
			players[i]->accelleration = 0;
		}
	}

	// Animated the uv offset of the materials
	materials[0]->uvOffset.x = sin(totalTime);
	materials[0]->uvOffset.y = cos(totalTime);

	camera->Update(deltaTime);

	audioHandler->Update(deltaTime, totalTime);

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	RenderManager::GetInstance()->Render(camera, context, lights);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
	{
		camera->transform->Rotate((float)(y - prevMousePos.y) * 0.1f, (float)(x - prevMousePos.x) * 0.1f, 0.0f);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
void Game::OnSuspending()
{
	audioHandler->Suspend();
}
void Game::OnResuming()
{
	audioHandler->Resume();
}
#pragma endregion