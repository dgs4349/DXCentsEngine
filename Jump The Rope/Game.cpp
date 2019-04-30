#include "Game.h"
#include "Vertex.h"
#include <DDSTextureLoader.h>

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
	delete particleVS;
	delete particlePS;
	delete flame1;
	

	blendState->Release();
	particleBlendState->Release();
	particleDepthState->Release();

	for (int i = 0; i < textureViews.size(); ++i)
	{
		textureViews[i]->Release();
	}

	hashTexture1_1->Release();
	hashTexture2_1->Release();
	hashTexture1_2->Release();
	hashTexture2_2->Release();
	hashTexture1_3->Release();
	hashTexture2_3->Release();

	hashSampler->Release();

	textureViews.clear();
	materials.clear();
	gameObjects.clear();
	meshes.clear();

	jumpSfx.clear();
	delete audioHandler;

	skySRV->Release();
	skyDepthState->Release();
	skyRasterState->Release();
	delete skyVS;
	delete skyPS;
	sampler->Release();
	// Clean up post process
	ppSRV->Release();
	ppRTV->Release();
	delete ppVS;
	delete ppPS;

	bloomSRV->Release();
	bloomRTV->Release();
	delete bloomPS;

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

	lights.ambientLights[0] = { Color(0.05f), 1 };
	lights.ambientLightCount = 1;

	//lights.dirLights[0] = { Color(.1f, .05f, .05f, 1), XMFLOAT3(1, 1, 0) };
	//lights.dirLightCount = 1;

	lights.pointLights[0] = { XMFLOAT3(0.960f, 0.3f, 0.2f), 10, XMFLOAT3(-8.0f, 0.45f, 8.0f), 2 };
	lights.pointLights[1] = { XMFLOAT3(0.960f, 0.3f, 0.2f), 10, XMFLOAT3(8.0f, 0.45f, 8.0f), 2 };
	lights.pointLightCount = 2;

	lights.spotLights[0] = { XMFLOAT3(.5f, .25f, 0), 25, XMFLOAT3(0, 8, -5), 10, XMFLOAT3(0, 1,-.5f), 1 };
	lights.spotLightCount = 1;

	/*ambientLight = { Color(0.5f) };
	directionalLight = { Color(0.5f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	directionalLight2 = { Color(0.0f, 0.0f, 0.5f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };*/


	audioHandler->Init();
	// loading audio here for now
	bgIntro = audioHandler->CreateSoundEffect(L"Assets/Audio/audio_background_intro.wav");
	bgLoop = audioHandler->CreateSoundEffect(L"Assets/Audio/audio_background_loop.wav", true);
	bgIntro->Link(bgLoop);
	bgIntro->Set(0.5f);
	bgIntro->PlayOnUpdate();

	jumpSfx.push_back(audioHandler->CreateSoundEffect(L"Assets/Audio/sfx/jump_3.wav"));
	jumpSfx.push_back(audioHandler->CreateSoundEffect(L"Assets/Audio/sfx/jump_2.wav"));
	jumpSfx.push_back(audioHandler->CreateSoundEffect(L"Assets/Audio/sfx/jump_1.wav"));
	jumpSfx.push_back(audioHandler->CreateSoundEffect(L"Assets/Audio/sfx/jump_0.wav"));
	for (int i = 0; i < jumpSfx.size(); i++) jumpSfx[i]->Set(2.0f);

	D3D11_RASTERIZER_DESC skyRD = {};
	skyRD.CullMode = D3D11_CULL_FRONT;
	skyRD.FillMode = D3D11_FILL_SOLID;
	skyRD.DepthClipEnable = true;
	device->CreateRasterizerState(&skyRD, &skyRasterState);

	D3D11_DEPTH_STENCIL_DESC skyDS = {};
	skyDS.DepthEnable = true;
	skyDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	skyDS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&skyDS, &skyDepthState);



	// Sampler state for post process
	// Create a sampler state that holds options for sampling
	// The descriptions should always just be local variables
	D3D11_SAMPLER_DESC samplerDesc = {}; // The {} part zeros out the struct!
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Setting this allows for mip maps to work! (if they exist)
	device->CreateSamplerState(&samplerDesc, &sampler);


	// Create post process resources -----------------------------------------
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// ppTexture is the link between pprtv and ppsrv
	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&textureDesc, 0, &ppTexture);


	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);

	// Create the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);

	// We don't need the texture reference itself no mo'
	ppTexture->Release();





	// BLOOM RESOURCES
	// Create post process resources -----------------------------------------
	D3D11_TEXTURE2D_DESC bloomTextureDesc = {};
	bloomTextureDesc.Width = width/4;
	bloomTextureDesc.Height = height/4;
	bloomTextureDesc.ArraySize = 1;
	bloomTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bloomTextureDesc.CPUAccessFlags = 0;
	bloomTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bloomTextureDesc.MipLevels = 1;
	bloomTextureDesc.MiscFlags = 0;
	bloomTextureDesc.SampleDesc.Count = 1;
	bloomTextureDesc.SampleDesc.Quality = 0;
	bloomTextureDesc.Usage = D3D11_USAGE_DEFAULT;

	// ppTexture is the link between pprtv and ppsrv
	ID3D11Texture2D* bloomTexture;
	device->CreateTexture2D(&bloomTextureDesc, 0, &bloomTexture);


	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC bloomRtvDesc = {};
	bloomRtvDesc.Format = bloomTextureDesc.Format;
	bloomRtvDesc.Texture2D.MipSlice = 0;
	bloomRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(bloomTexture, &bloomRtvDesc, &bloomRTV);

	// Create the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC bloomSrvDesc = {};
	bloomSrvDesc.Format = bloomTextureDesc.Format;
	bloomSrvDesc.Texture2D.MipLevels = 1;
	bloomSrvDesc.Texture2D.MostDetailedMip = 0;
	bloomSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(bloomTexture, &bloomSrvDesc, &bloomSRV);

	// We don't need the texture reference itself no mo'
	bloomTexture->Release();

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
	pixelShader->LoadShaderFile(L"PixelShaderHash.cso");

	particleVS = new SimpleVertexShader(device, context);
	particleVS->LoadShaderFile(L"ParticleVS.cso");

	particlePS = new SimplePixelShader(device, context);
	particlePS->LoadShaderFile(L"ParticlePS.cso");

	skyVS = new SimpleVertexShader(device, context);
	skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, context);
	skyPS->LoadShaderFile(L"SkyPS.cso");

	// Post process stuff
	ppVS = new SimpleVertexShader(device, context);
	ppVS->LoadShaderFile(L"PostProcessVS.cso");

	ppPS = new SimplePixelShader(device, context);
	ppPS->LoadShaderFile(L"PostProcessPS.cso");

	bloomPS = new SimplePixelShader(device, context);
	bloomPS->LoadShaderFile(L"BloomPS.cso");
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
	meshes.push_back(new Mesh("Assets/Models/Torch.obj", device));
}

void Game::LoadTextures()
{
	//does hashmark shading
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_light.dds", 0, &hashTexture1_1);
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_dark.dds", 0, &hashTexture2_1);
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_light2.dds", 0, &hashTexture1_2);
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_dark2.dds", 0, &hashTexture2_2);
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_light3.dds", 0, &hashTexture1_3);
	CreateDDSTextureFromFile(device, context, L"Assets/Textures/Hashing/hashing_biggest_dark3.dds", 0, &hashTexture2_3);

	//all other textures
	ID3D11ShaderResourceView* texView1;		// Cobblestone
	ID3D11ShaderResourceView* texView2;		// Dirt
	ID3D11ShaderResourceView* texView3;		// StonePath
	ID3D11ShaderResourceView* texView4;		// WoodCrate
	ID3D11ShaderResourceView* texView5;		// skeleton texture
	ID3D11ShaderResourceView* texView6;		// ghost texture 1 (P1)
	ID3D11ShaderResourceView* texView7;		// ghost texture 2 (P2)
	ID3D11ShaderResourceView* texView8;		// ghost texture 3 (P1 lighter)
	ID3D11ShaderResourceView* texView9;		// ghost texture 4 (P2 lighter)
	ID3D11ShaderResourceView* texView10;	// ghost texture 5 (red aka failed)
	ID3D11ShaderResourceView* texView11;	// ground
	ID3D11ShaderResourceView* texView14;	// test fire particle
	ID3D11ShaderResourceView* texView13;	// ground normal map
	ID3D11ShaderResourceView* texView12;	// fog
	ID3D11ShaderResourceView* texView15;	// flame
	ID3D11ShaderResourceView* texView16;	// basic particle
	ID3D11ShaderResourceView* texView17;	// smoke particle


	CreateDDSTextureFromFile(device, L"Assets/Textures/SunnyCubeMap.dds", 0, &skySRV);

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

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Fog.png", 0, &texView12);
	textureViews.push_back(texView12);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/Ground_Normal.png", 0, &texView13);
	textureViews.push_back(texView13);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/fireParticle.jpg", 0, &texView14);
	textureViews.push_back(texView14);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/fire.png", 0, &texView15);
	textureViews.push_back(texView15);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/particleTest.png", 0, &texView16);
	textureViews.push_back(texView16);

	CreateWICTextureFromFile(device, context, L"Assets/Textures/smoke.png", 0, &texView17);
	textureViews.push_back(texView17);

	ID3D11ShaderResourceView * blankNormal;
	CreateWICTextureFromFile(device, context, L"Assets/Textures/normal.png", 0, &blankNormal);
	textureViews.push_back(blankNormal);

	ID3D11ShaderResourceView * fogNormal;
	CreateWICTextureFromFile(device, context, L"Assets/Textures/Fog_Normal.png", 0, &fogNormal);
	textureViews.push_back(fogNormal);

}

void Game::SetShaderHashTextures(float deltaTime)
{
	pixelShader->SetSamplerState("hashSampler", hashSampler);

	animTimer -= deltaTime;
	if (animTimer <= 0) {
		animTimer = animSpeed;
		animFrame++;
		if (animFrame > 2)
			animFrame = 0;
	}

	switch (animFrame) {
	case 0:
		pixelShader->SetShaderResourceView("hashTexture1", hashTexture1_1);
		pixelShader->SetShaderResourceView("hashTexture2", hashTexture2_1);
		break;
	case 1:
		pixelShader->SetShaderResourceView("hashTexture1", hashTexture1_2);
		pixelShader->SetShaderResourceView("hashTexture2", hashTexture2_2);
		break;
	case 2:
		pixelShader->SetShaderResourceView("hashTexture1", hashTexture1_3);
		pixelShader->SetShaderResourceView("hashTexture2", hashTexture2_3);
		break;
	default:
		pixelShader->SetShaderResourceView("hashTexture1", hashTexture1_1);
		pixelShader->SetShaderResourceView("hashTexture2", hashTexture2_1);
		break;
	}
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

	// Blend state
	D3D11_BLEND_DESC bd = {};
	bd.RenderTarget[0].BlendEnable = true;

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Turns off depth writing
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&dsDesc, &particleDepthState);

	// Blend for particles (additive)
	D3D11_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable = false;
	blend.IndependentBlendEnable = false;
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Still respect pixel shader output alpha
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blend, &particleBlendState);

	// These control how the RGB channels are combined
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// These control how the alpha channel is combined
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&bd, &blendState);

	// Set the blend state (using a bit mask of 0xFFFFFFFF for sample mask)
	context->OMSetBlendState(blendState, 0, 0xFFFFFFFF);

	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[0], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[1], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[2], textureViews[11], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[3], samplerState));

	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[4], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[5], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[6], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[7], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[8], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[9], textureViews[18], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[10], textureViews[13], samplerState));
	device->CreateSamplerState(&samplerDesc, &samplerState);
	materials.push_back(new Material(vertexShader, pixelShader, textureViews[12], textureViews[19], samplerState));
	
	//defines a separate sampler for the hash marks. It requires blending between mips
	D3D11_SAMPLER_DESC hashDesc;
	ZeroMemory(&hashDesc, sizeof(D3D11_SAMPLER_DESC));
	hashDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	hashDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	hashDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hashDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hashDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&hashDesc, &hashSampler);
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
	cross = new GameObject("Cross", meshes[12], materials[10]);
	grave1 = new GameObject("Grave1", meshes[13], materials[10]);
	grave2 = new GameObject("Grave2", meshes[13], materials[10]);
	grave3 = new GameObject("Grave3", meshes[13], materials[10]);
	grave4 = new GameObject("Grave4", meshes[13], materials[10]);
	grave5 = new GameObject("Grave5", meshes[13], materials[10]);

	flame1 = new Flame(XMFLOAT3(-8.0f, 0.45f, 8.0f), textureViews[17], textureViews[15], textureViews[16], device, particleVS, particlePS);

	// Skeletons
	skel1 = new GameObject("Skeleton1", meshes[7], materials[4]);
	skel2 = new GameObject("Skeleton2", meshes[7], materials[4]);
	skel1Arm = new GameObject("Skeleton1Arm", meshes[8], materials[4]);
	skel2Arm = new GameObject("Skeleton2Arm", meshes[8], materials[4]);
	torch = new GameObject("Torch", meshes[14], materials[10]);
	// Fog
	fog = new GameObject("Fog", meshes[1], materials[11]);



	torch->transform->Position(-8.0f, 0.3f, 8.0f);

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

	fog->transform->Scale(35.0f, .5f, 35.0f);
	fog->transform->Position(0.0f, -1.0f, 0.0f);
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
	SetShaderHashTextures(deltaTime);

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

				numJumps = 0;

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
		if (rope->transform->EulerAngles().x < 90) {
			awardedJump = false;
		}
		else if (rope->transform->EulerAngles().x > 180 - ropeWidth && rope->transform->EulerAngles().x < 180 + ropeWidth)
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
		else if (rope->transform->EulerAngles().x > 225 && !awardedJump) {
			if (numJumps > 4) jumpSfx[(numJumps - 2) % 4]->Stop(true);
			jumpSfx[numJumps % 4]->Play();
			numJumps++;
			awardedJump = true;
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
	skel1Arm->transform->Rotate({ 0, 0, ropeSpeed * deltaTime });
	skel2Arm->transform->Rotate({ 0, 0, -ropeSpeed * deltaTime });

	flame1->Update(deltaTime);

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

	//basic light modulation
	lights.pointLights[0].range = sin(totalTime) + 12.0f;

	// Animated the uv offset of the materials
	materials[11]->uvOffset.x = totalTime * .1f;
	materials[11]->uvOffset.y = cos(totalTime) / 20.0f;


	camera->Update(deltaTime);

	//audioHandler->Update(deltaTime, totalTime);


	// animate the torches light
	lights.pointLights[0].color = XMFLOAT3(1.0f, 0.3f, 0.1f);
	lights.pointLights[0].intensity = sin(totalTime * 10.54f) * sin(totalTime * 3.78f) * sin(totalTime * 2.487f) * sin(totalTime * 0.879f) * 3 + 4;


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
	//const float color[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	//const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// POST PROCESS PRE-RENDER /////////////////////////////
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11ShaderResourceView* nullSRVs[16] = {};
	ID3D11Buffer* nothing = 0;

	// Also clear the post process texture
	context->ClearRenderTargetView(ppRTV, color);
	context->ClearRenderTargetView(bloomRTV, color);
	// Set the post process RTV as the current render target
	context->OMSetRenderTargets(1, &ppRTV, depthStencilView);

	// render normally, to ppRTV
	RenderManager::GetInstance()->Render(camera, context, lights);

	float blend[4] = { 1,1,1,1 };
	context->OMSetBlendState(particleBlendState, blend, 0xffffffff);	// Additive blending
	context->OMSetDepthStencilState(particleDepthState, 0);

	//flame1->Draw(context, camera);

	context->OMSetBlendState(blendState, blend, 0xffffffff);
	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(0);

	DrawSky();

	// POST PROCESS POST-RENDER ///////////////////////////
	
	
	// render bloom

	context->OMSetRenderTargets(1, &bloomRTV, 0);
	// Render a full-screen triangle using the post process shaders
	ppVS->SetShader();

	bloomPS->SetShader();
	bloomPS->SetShaderResourceView("Pixels", ppSRV);
	bloomPS->SetSamplerState("Sampler", sampler);

	bloomPS->SetFloat("pixelWidth", 1.0f / width );
	bloomPS->SetFloat("pixelHeight", 1.0f / height  );
	bloomPS->SetInt("blurAmount", 5);
	bloomPS->CopyAllBufferData();

	// Deactivate vertex and index buffers
	
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	// Draw a set number of vertices
	context->Draw(3, 0);
	context->PSSetShaderResources(0, 16, nullSRVs);
	
	
	
	// Render to the screen
	// Set the target back to the back buffer
	context->OMSetRenderTargets(1, &backBufferRTV, 0);

	// Render a full-screen triangle using the post process shaders
	ppVS->SetShader();

	ppPS->SetShader();
	ppPS->SetShaderResourceView("Pixels", ppSRV);
	ppPS->SetShaderResourceView("Bloom", bloomSRV);
	ppPS->SetSamplerState("Sampler", sampler);

	ppPS->SetFloat("pixelWidth", 1.0f / width);
	ppPS->SetFloat("pixelHeight", 1.0f / height);
	ppPS->SetInt("blurAmount", 25);
	ppPS->CopyAllBufferData();

	// Deactivate vertex and index buffers
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	// Draw a set number of vertices
	context->Draw(3, 0);

	// Unbind all pixel shader SRVs
	context->PSSetShaderResources(0, 16, nullSRVs);
	
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}

void Game::DrawSky()
{
	// Set the vertex and index buffers

	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, meshes[1]->GetVertexBuffer(), &stride, &offset);
	context->IASetIndexBuffer(meshes[1]->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);

	// Set up the shaders
	skyVS->SetMatrix4x4("view", camera->ViewMatrix());
	skyVS->SetMatrix4x4("projection", camera->ProjectionMatrix());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();

	skyPS->SetShaderResourceView("Sky", skySRV);
	skyPS->SetSamplerState("BasicSampler", materials[0]->SamplerState());
	skyPS->SetShader();

	// Set up any new render states
	context->RSSetState(skyRasterState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	// Draw
	context->DrawIndexed(meshes[1]->GetIndexCount(), 0, 0);

	// Reset states
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

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