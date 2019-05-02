#include "Game.h"
#include "LightManager.h"
#include "Vertex.h"
#include "MAT_PBRTexture.h"
#include "MAT_Water.h"
#include "MAT_Skybox.h"
#include "MAT_Basic.h"

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
	//Delete sampler states
	samplerState->Release();
	waterSamplerState->Release();
	shadowSampler->Release();

	//Delete the camera
	if (camera) { delete camera; }
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	//Load all needed assets
	resourceManager = ResourceManager::GetInstance();
	LoadAssets();

	//Initialize singletons
	inputManager = InputManager::GetInstance();
	renderer = Renderer::GetInstance();
	renderer->Init(device, width, height);
	entityManager = EntityManager::GetInstance();
	swimmerManager = SwimmerManager::GetInstance();
	swimmerManager->SetLevelRadius(LEVEL_RADIUS - 1);

	//Initialize singleton data
	inputManager->Init(hWnd);

	//Create game entities
	gameState = GameState::Menu;
	CreateEntities();

	//Initialize transformation modifiers
	position = -2;
	rotation = 0;
	rotSpeed = 20;
	scale = 1;

	//Water
	translate = 0.0f;

	//Initialize lights
	//Set ambient light
	LightManager* lightManager = LightManager::GetInstance();
	lightManager->SetAmbientColor(0.35f, 0.19f, 0.02f);

	//Directional lights
	DirectionalLight* dLight = lightManager->CreateDirectionalLight(true, XMFLOAT3(1, 1, 1), 1);
	dLight->SetRotation(60, -45, 0);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::LoadAssets()
{
	//Load shaders
	resourceManager->LoadVertexShader("VertexShader.cso", device, context);
	resourceManager->LoadPixelShader("PixelShader.cso", device, context);

	resourceManager->LoadPixelShader("PS_Water.cso", device, context);
	resourceManager->LoadPixelShader("PS_ShineWater.cso", device, context);

	resourceManager->LoadVertexShader("VS_ColDebug.cso", device, context);
	resourceManager->LoadPixelShader("PS_ColDebug.cso", device, context);

	resourceManager->LoadVertexShader("FXAAShaderVS.cso", device, context);
	resourceManager->LoadPixelShader("FXAAShaderPS.cso", device, context);

	resourceManager->LoadVertexShader("VS_Sky.cso", device, context);
	resourceManager->LoadPixelShader("PS_Sky.cso", device, context);

	resourceManager->LoadVertexShader("VS_Shadow.cso", device, context);

	//Create meshes
	resourceManager->LoadMesh("Assets\\Models\\cube.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\boat.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\swimmer.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\area.obj", device);

	//Load textures
	resourceManager->LoadTexture2D("Assets/Textures/Boat/boat_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Boat/boat_normals.png", device, context);

	resourceManager->LoadTexture2D("Assets/Textures/Swimmer/swimmer_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Swimmer/swimmer_normals.png", device, context);

	resourceManager->LoadTexture2D("Assets/Textures/Area/area_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Area/area_normals.png", device, context);

	resourceManager->LoadTexture2D("Assets/Textures/Water/blue.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Water/water_normal_1.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Water/water_normal_2.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Water/water_normal_3.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Water/water_metal.png", device, context);

	//Load cubemaps
	resourceManager->LoadCubeMap("Assets/Textures/Sky/SunnyCubeMap.dds", device);
	resourceManager->LoadCubeMap("Assets/Textures/Water/water_shine.dds", device);

	//Create sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //Anisotropic filtering
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //Anisotropic filtering
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 1.8f;
	device->CreateSamplerState(&samplerDesc, &waterSamplerState);

	// Create the special "comparison" sampler state for shadows
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; // Could be anisotropic
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSampler);

	SimpleVertexShader* vs = resourceManager->GetVertexShader("VertexShader.cso");
	SimplePixelShader* ps_basic = resourceManager->GetPixelShader("PixelShader.cso");

	//Boat material
	Material* mat_boat = new MAT_Basic(vs, ps_basic, XMFLOAT2(1, 1), samplerState,
		resourceManager->GetTexture2D("Assets/Textures/Boat/boat_albedo.png"),
		resourceManager->GetTexture2D("Assets/Textures/Boat/boat_normals.png"),
		0, 50, shadowSampler);
	resourceManager->AddMaterial("boat", mat_boat);

	//Swimmer Material
	Material* mat_swimmer = new MAT_Basic(vs, ps_basic, XMFLOAT2(1, 1), samplerState,
		resourceManager->GetTexture2D("Assets/Textures/Swimmer/swimmer_albedo.png"),
		resourceManager->GetTexture2D("Assets/Textures/Swimmer/swimmer_normals.png"),
		0, 50, shadowSampler);
	resourceManager->AddMaterial("swimmer", mat_swimmer);

	//Area Material
	Material* mat_area = new MAT_Basic(vs, ps_basic, XMFLOAT2(1, 1), samplerState,
		resourceManager->GetTexture2D("Assets/Textures/Area/area_albedo.png"),
		resourceManager->GetTexture2D("Assets/Textures/Area/area_normals.png"),
		0, 50, shadowSampler);
	resourceManager->AddMaterial("area", mat_area);
	
	//Water surface material
	Material* mat_water = new MAT_Water(vs, resourceManager->GetPixelShader("PS_ShineWater.cso"),
		XMFLOAT2(2, 2), waterSamplerState, 
		resourceManager->GetTexture2D("Assets/Textures/Water/blue.png"),
		resourceManager->GetTexture2D("Assets/Textures/Water/water_normal_2.png"),
		0, 100, shadowSampler, &translate,
		resourceManager->GetTexture2D("Assets/Textures/Water/water_normal_3.png"),
		resourceManager->GetCubeMap("Assets/Textures/Water/water_shine.dds"));
	resourceManager->AddMaterial("water", mat_water);

	//Skybox material
	Material* mat_skybox = new MAT_Skybox(ResourceManager::GetInstance()->GetVertexShader("VS_Sky.cso"),
		ResourceManager::GetInstance()->GetPixelShader("PS_Sky.cso"),
		ResourceManager::GetInstance()->GetCubeMap("Assets/Textures/Sky/SunnyCubeMap.dds"),
		samplerState
	);
	resourceManager->AddMaterial("skybox", mat_skybox);
}

void Game::CreateEntities()
{
	//Create water
	/*Entity* water = new Entity(
		resourceManager->GetMesh("Assets\\Models\\cube.obj"),
		resourceManager->GetMaterial("water")
	);
	water->SetScale(26, 0.1f, 26);*/

	//Create area
	Entity* area = new Entity(resourceManager->GetMesh("Assets\\Models\\area.obj"),
		resourceManager->GetMaterial("area"));
	area->SetScale(2.18f, 0.5f, 2.18f);

	// Player (Boat) - Create the player.
	player = new Boat(
		resourceManager->GetMesh("Assets\\Models\\boat.obj"),
		resourceManager->GetMaterial("boat"),
		LEVEL_RADIUS
	);
	player->SetPosition(0, 0, 0); // Set the player's initial position.
	player->AddCollider(XMFLOAT3(0.9f, 0.8f, 2.3f), XMFLOAT3(0, 0, 0));
#if defined(DEBUG) || defined(_DEBUG)
	player->SetDebug(true);
#endif

	//Create the camera and initialize matrices
	camera = new FocusCamera(player, XMFLOAT3(0, 16, -23), XMFLOAT3(40.75f, 0, 0), 4, 2);
	camera->CreateProjectionMatrix(0.25f * XM_PI, (float)width / height, 0.1f, 100.0f);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->CreateProjectionMatrix(
		0.25f * XM_PI,			// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	renderer->CreatePostProcessingResources(device, width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	inputManager->UpdateFocus();
	if (!inputManager->IsWindowFocused())
		return;

	//The only call to UpdateMousePos() for the InputManager
	//Get the current mouse position
	inputManager->UpdateMousePos();
	// --------------------------------------------------------
	//All game code goes below


	// Quit if the escape key is pressed
	if (inputManager->GetKey(VK_ESCAPE))
		Quit();

	//Update the camera
	camera->Update(deltaTime);
	
	//Gamestate switch
	switch (gameState)
	{
		case GameState::Menu:
			gameState = GameState::Playing;
			break;

		case GameState::Playing:
			// Updates the swimmer generator/manager.
			if(player->GetState() == BoatState::Playing)
				swimmerManager->Update(deltaTime);

			entityManager->Update(deltaTime);

			//Check for gameover
			if (player->GetState() == BoatState::Crashed)
				gameState = GameState::GameOver;
			break;

		case GameState::GameOver:
			entityManager->Update(deltaTime);

			//Check for reset input
			if (inputManager->GetKey(VK_SPACE))
			{
				player->Reset();
				swimmerManager->Reset();
				gameState = GameState::Playing;
			}
			break;

		default:
			break;
	}

	//Updates water's scrolling normal map
	translate += 0.025f * deltaTime;
	if (translate > 1.0f) translate = 0.0f;
	

	//All game code goes above
	// --------------------------------------------------------
	//The only call to Update() for the InputManager
	//Update for next frame
	inputManager->UpdateStates();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	//Draw all entities in the renderer
	renderer->SetClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Needed for clearing the post process buffer texture and the back buffer.
	renderer->Draw(context, device, camera, backBufferRTV, depthStencilView, samplerState, width, height);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	inputManager->OnMouseDown(buttonState, x, y);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y, int button)
{
	inputManager->OnMouseUp(buttonState, x, y, button);
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	inputManager->OnMouseMove(buttonState, x, y);
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.
// WheelDelta may be positive or negative, depending
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	inputManager->OnMouseWheel(wheelDelta, x, y);
}
#pragma endregion