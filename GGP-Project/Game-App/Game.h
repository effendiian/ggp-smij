#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "Renderer.h"
#include "InputManager.h"
#include "FirstPersonCamera.h"
#include "WICTextureLoader.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	//Camera
	FirstPersonCamera* camera;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y, int button);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:
	//Singletons
	Renderer* renderer;
	InputManager* inputManager;

	//Meshes and entities
	Mesh* meshes[4];
	Entity* entities[5];

	//Materials and textures
	Material* materials[2];
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* srvs[2];

	//Transformation modifiers
	float position;
	float rotation;
	float rotSpeed;
	float scale;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateBasicGeometry();
	void CreateEntities();
};

