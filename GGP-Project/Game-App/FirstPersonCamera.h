#pragma once
#include "Camera.h"
#include "InputManager.h"

// --------------------------------------------------------
// A first person camera definition. Includes flying movement
// --------------------------------------------------------
class FirstPersonCamera : public Camera
{
private:
	InputManager* inputManager;

	// --------------------------------------------------------
	// Calculate the camera's rotation when moving
	// --------------------------------------------------------
	void CalculateCameraRotFromMouse();

public:
	// --------------------------------------------------------
	// Constructor - Set up the first person camera
	// (Remember to create the projection matrix right after!)
	// --------------------------------------------------------
	FirstPersonCamera();

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~FirstPersonCamera();

	// --------------------------------------------------------
	// Update the camera (runs every frame)
	//
	// deltaTime - The time between frames
	// moveSpeed - The speed the camera moves at (defaulted to 1)
	// --------------------------------------------------------
	void Update(float deltaTime, float moveSpeed = 10);
};

