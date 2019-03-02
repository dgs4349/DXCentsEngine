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
	Logger::GetInstance();
	ObjectManager::GetInstance();
	RenderManager::GetInstance();

	camObject = new GameObject("Camera");

	camera = camObject->AddComponent<Camera>();
	directionalLight = DirectionalLight();
	directionalLight2 = DirectionalLight();

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

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

	camera->transform->Position(0.0f, 0.0f, -10.0f);
	camera->SetScreenSize(width, height);

	ambientLight = { Color(0.5f) };
	directionalLight = { Color(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	directionalLight2 = { Color(0.0f, 0.0f, 0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
}

void Game::LoadTextures()
{
	ID3D11ShaderResourceView* texView1;
	ID3D11ShaderResourceView* texView2;
	ID3D11ShaderResourceView* texView3;
	ID3D11ShaderResourceView* texView4;

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Cobblestone.jpg", 0, &texView1);
	textureViews.push_back(texView1);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Dirt.jpg", 0, &texView2);
	textureViews.push_back(texView2);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/StonePath.jpg", 0, &texView3);
	textureViews.push_back(texView3);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/WoodCrate.jpg", 0, &texView4);
	textureViews.push_back(texView4);
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
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[0], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[1], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[2], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[3], samplerState));
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	gameObjects.push_back(new GameObject("Test", meshes[0], materials[0]));
	gameObjects.push_back(new GameObject("Test", meshes[1], materials[3]));
	gameObjects.push_back(new GameObject("Test", meshes[2], materials[2]));
	gameObjects.push_back(new GameObject("Test", meshes[3], materials[1]));
	gameObjects.push_back(new GameObject("Test", meshes[4], materials[2]));
	gameObjects.push_back(new GameObject("Test", meshes[5], materials[3]));

	gameObjects[1]->transform->Position(2.0f, 0.0f, 0.0f);
	gameObjects[2]->transform->Position(4.0f, 0.0f, 0.0f);
	gameObjects[3]->transform->Position(6.0f, 0.0f, 0.0f);
	gameObjects[4]->transform->Position(8.0f, 0.0f, 0.0f);
	gameObjects[5]->transform->Position(10.0f, 0.0f, 0.0f);
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
	camera->Update(deltaTime);

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

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->GetComponent<MeshRenderer>()->GetMaterial()->PixelShader()->SetData("ambientLight", &ambientLight, sizeof(AmbientLight));
		gameObjects[i]->GetComponent<MeshRenderer>()->GetMaterial()->PixelShader()->SetData("light", &directionalLight, sizeof(DirectionalLight));
		gameObjects[i]->GetComponent<MeshRenderer>()->GetMaterial()->PixelShader()->SetData("light2", &directionalLight2, sizeof(DirectionalLight));
		gameObjects[i]->GetComponent<MeshRenderer>()->PrepareMaterial(camera->ViewMatrix(), camera->ProjectionMatrix());

		context->IASetVertexBuffers(0, 1, gameObjects[i]->GetComponent<MeshFilter>()->GetMesh()->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(gameObjects[i]->GetComponent<MeshFilter>()->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(gameObjects[i]->GetComponent<MeshFilter>()->GetMesh()->GetIndexCount(), 0, 0);
	}

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
		camera->transform->Rotate((float)(y - prevMousePos.y) * 0.001f, (float)(x - prevMousePos.x) * 0.001f, 0.0f);
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
#pragma endregion