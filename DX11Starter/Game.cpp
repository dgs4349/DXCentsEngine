#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	camera = new Camera();

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
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete camera;

	for (int i = 0; i < 5; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (meshes[i] != nullptr)
		{
			delete meshes[i];
		}
	}
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
	CreateBasicGeometry();

	camera->transform.Position(0.0f, 0.0f, -10.0f);

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

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 pink = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), red },
		{ XMFLOAT3(1.5f, -1.0f, 0.0f), blue },
		{ XMFLOAT3(-1.5f, -1.0f, 0.0f), green },
	};

	Vertex vertices2[] =
	{
	{ XMFLOAT3(1.0f, 1.0f, 0.0f), red },
	{ XMFLOAT3(-1.0f, 1.0f, 0.0f), blue },
	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), green },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), white },
	};

	Vertex vertices3[] =
	{
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), red },
	{ XMFLOAT3(-1.0f, 1.0f, 0.0f), blue },
	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), green },
	{ XMFLOAT3(1.0f, -1.0f, 0.0f), white },
	{ XMFLOAT3(0.0f, -2.0f, 0.0f), pink },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	uint16_t indices[] = { 0, 1, 2 };
	uint16_t indices2[] = { 1, 0, 2, 0, 3, 2 };
	uint16_t indices3[] = { 1, 0, 2, 0, 3, 2, 2, 3, 4 };

	meshes[0] = new Mesh(vertices, 3, indices, 3, device);
	meshes[1] = new Mesh(vertices2, 4, indices2, 6, device);
	meshes[2] = new Mesh(vertices3, 5, indices3, 9, device);

	entities[0] = new Entity(meshes[0]);
	entities[1] = new Entity(meshes[0]);
	entities[2] = new Entity(meshes[1]);
	entities[3] = new Entity(meshes[1]);
	entities[4] = new Entity(meshes[2]);

	entities[0]->transform.Position(-2.0f, -1.0f, 0.01f);
	entities[1]->transform.Position(2.0f, -1.0f, 0.0f);

	entities[2]->transform.Position(-2.5f, 1.0f, 0.0f);
	entities[2]->transform.Scale(0.5f, 0.5f, 1);
	entities[3]->transform.Position(2.5f, 1.0f, 0.0f);
	entities[3]->transform.Scale(0.5f, 0.5f, 1);

	entities[4]->transform.Position(0.0f, 0.11f, 0.0f);
	entities[4]->transform.Scale(0.1f, .75f, 1);
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

float i = 0;
float x = 0;

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	entities[0]->transform.Position(sin(i) * 3, entities[0]->transform.Position().y, entities[0]->transform.Position().z);
	entities[1]->transform.Position(cos(i) * 3, entities[1]->transform.Position().y, entities[1]->transform.Position().z);
	entities[2]->transform.Rotate(0.0f, 0.0f, 0.01f);
	entities[3]->transform.Rotate(-0.01f, 0.0f, 0.0f);
	entities[4]->transform.Rotate(0.0f, 0.0f, 0.001f);

	i += deltaTime;
	x += deltaTime;

	DirectX::XMFLOAT3 pos = entities[4]->transform.Up();

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

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	vertexShader->SetMatrix4x4("view", camera->ViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera->ProjectionMatrix());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < 5; ++i)
	{
		vertexShader->SetMatrix4x4("world", entities[i]->transform.WorldMatrix());
		vertexShader->CopyAllBufferData();

		context->IASetVertexBuffers(0, 1, entities[i]->GetMesh()->GetVertexBuffer(), &stride, &offset);
		context->IASetIndexBuffer(entities[i]->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(entities[i]->GetMesh()->GetIndexCount(), 0, 0);
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
		camera->transform.Rotate((float)(y - prevMousePos.y) * 0.001f, (float)(x - prevMousePos.x) * 0.001f, 0.0f);
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