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
	//Delete sampler state
	samplerState->Release();

	//Delete entities - Refactored into EntityManager.
	// for (int i = 0; i < entities.size(); i++)
	// {
	// 	if (entities[i]) { delete entities[i]; }
	// }

	//Delete the camera
	if (camera) { delete camera; }
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	//Initialize singletons
	inputManager = InputManager::GetInstance();
	renderer = Renderer::GetInstance();
	resourceManager = ResourceManager::GetInstance();
	entityManager = EntityManager::GetInstance();
	swimmerManager = SwimmerManager::GetInstance();

	//Initialize singleton data
	inputManager->Init(hWnd);

	//Create the camera and initialize matrices
	camera = new FirstPersonCamera();
	camera->CreateProjectionMatrix(0.25f * XM_PI, (float)width / height, 0.1f, 100.0f);
	camera->SetRotation(75, 0, 0);
	camera->SetPosition(0, 25, -5);

	//Load all needed assets
	LoadAssets();
	CreateEntities();

	//Initialize transformation modifiers
	position = -2;
	rotation = 0;
	rotSpeed = 20;
	scale = 1;

	//Initialize lights
	//Set ambient light
	LightManager* lightManager = LightManager::GetInstance();
	lightManager->SetAmbientColor(0.01f, 0.01f, 0.01f);

	//Directional lights
	lightManager->CreateDirectionalLight(XMFLOAT3(1, 1, 1), 1);

	////Point light
	//PointLight* pLight = lightManager->CreatePointLight(5, XMFLOAT3(0, 1, 0), 1);
	//pLight->SetPosition(0, -2, 3);

	////Spot light
	//SpotLight* sLight = lightManager->CreateSpotLight(5, 5, XMFLOAT3(0, 0, 1), 1);
	//sLight->SetPosition(2, 0, -1);
	//sLight->SetRotation(0, -90, 0);

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
	resourceManager->LoadPixelShader("PixelShaderPBR.cso", device, context);

	//Create meshes
	resourceManager->LoadMesh("Assets\\Models\\torus.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\cube.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\helix.obj", device);
	resourceManager->LoadMesh("Assets\\Models\\sphere.obj", device);

	//Load textures
	resourceManager->LoadTexture2D("Assets/Textures/Floor/floor_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Floor/floor_normals.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Floor/floor_roughness.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Floor/floor_metal.png", device, context);

	resourceManager->LoadTexture2D("Assets/Textures/Scratched/scratched_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Scratched/scratched_normals.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Scratched/scratched_roughness.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Scratched/scratched_metal.png", device, context);

	resourceManager->LoadTexture2D("Assets/Textures/Wood/wood_albedo.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Wood/wood_normals.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Wood/wood_roughness.png", device, context);
	resourceManager->LoadTexture2D("Assets/Textures/Wood/wood_metal.png", device, context);

	//Create sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //Anisotropic filtering
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	//Create materials
	SimpleVertexShader* vs = resourceManager->GetVertexShader("VertexShader.cso");
	SimplePixelShader* ps = resourceManager->GetPixelShader("PixelShaderPBR.cso");

	Material* mat1 = new MAT_PBRTexture(vs, ps, 1024.0f, XMFLOAT2(2, 2),
		resourceManager->GetTexture2D("Assets/Textures/Floor/floor_albedo.png"), 
		resourceManager->GetTexture2D("Assets/Textures/Floor/floor_normals.png"),
		resourceManager->GetTexture2D("Assets/Textures/Floor/floor_roughness.png"),
		resourceManager->GetTexture2D("Assets/Textures/Floor/floor_metal.png"),
		samplerState);
	resourceManager->AddMaterial("floor", mat1);

	Material* mat2 = new MAT_PBRTexture(vs, ps, 1024.0f, XMFLOAT2(2, 2),
		resourceManager->GetTexture2D("Assets/Textures/Scratched/scratched_albedo.png"),
		resourceManager->GetTexture2D("Assets/Textures/Scratched/scratched_normals.png"),
		resourceManager->GetTexture2D("Assets/Textures/Scratched/scratched_roughness.png"),
		resourceManager->GetTexture2D("Assets/Textures/Scratched/scratched_metal.png"),
		samplerState);
	resourceManager->AddMaterial("scratched", mat2);

	Material* mat3 = new MAT_PBRTexture(vs, ps, 1024.0f, XMFLOAT2(2, 2),
		resourceManager->GetTexture2D("Assets/Textures/Wood/wood_albedo.png"),
		resourceManager->GetTexture2D("Assets/Textures/Wood/wood_normals.png"),
		resourceManager->GetTexture2D("Assets/Textures/Wood/wood_roughness.png"),
		resourceManager->GetTexture2D("Assets/Textures/Wood/wood_metal.png"),
		samplerState);
	resourceManager->AddMaterial("wood", mat3);
}

void Game::CreateEntities()
{
	// Player (Boat) - Create the player.
	Entity* player = new Boat(
		resourceManager->GetMesh("Assets\\Models\\cube.obj"),
		resourceManager->GetMaterial("scratched")
	);
	player->SetPosition(0, 0, 0); // Set the player's initial position.
	player->AddCollider(XMFLOAT3(1.1f, 1.1f, 1.1f), XMFLOAT3(0, 0, 0));
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
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	//The only call to UpdateMousePos() for the InputManager
	//Get the current mouse position
	inputManager->UpdateMousePos();
	// --------------------------------------------------------

	// Quit if the escape key is pressed
	if (inputManager->GetKey(VK_ESCAPE))
		Quit();

	//Update the camera
	camera->Update(deltaTime);

	// Updates all the entities
	entityManager->Update(deltaTime);

	// Updates the swimmer generator/manager.
	swimmerManager->Update(deltaTime);
	
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
	// Background color (Cornflower Blue in this case) for clearing
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

	//Draw all entities in the renderer
	renderer->Draw(context, camera);

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