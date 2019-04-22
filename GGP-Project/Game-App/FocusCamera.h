#pragma once
#include "Camera.h"
#include "InputManager.h"

// --------------------------------------------------------
// A first person camera definition. Includes flying movement
// --------------------------------------------------------
class FocusCamera : public Camera
{
private:
	InputManager* inputManager;

	//Focus vars
	GameObject* focusObj;
	DirectX::XMFLOAT3 anchorPos;
	DirectX::XMFLOAT4 anchorRot;
	float zoom;
	float maxZoom;
	float zoomTick;
	float yMin;
	bool moveIn;

#if defined(DEBUG) || defined(_DEBUG)
	//Allow first person movement in debug mode
	float moveSpeed = 10;
	bool fpsMovement = false; //0 = focus, 1 = FPS
	bool prevKeypress;

	// --------------------------------------------------------
	// Calculate the camera's rotation when moving
	// --------------------------------------------------------
	void CalculateCameraRotFromMouse();

	// --------------------------------------------------------
	// Allow fps movement for debugging
	// --------------------------------------------------------
	void FPSMovement(float deltaTime);
#endif

public:
	// --------------------------------------------------------
	// Constructor - Set up the focus camera
	// (Remember to create the projection matrix right after!)
	//
	// obj - the object to focus on
	// anchor - the anchor position (minimum zoom)
	// anchorRot - the rotation at the origin
	// zoomDist - the max zoom distance to the focus object
	// yMinimum - the minimum y value
	// --------------------------------------------------------
	FocusCamera(GameObject* obj, DirectX::XMFLOAT3 anchor, 
		DirectX::XMFLOAT3 anchorRotation, float zoomDist,
		float yMinimum);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~FocusCamera();

	// --------------------------------------------------------
	// Update the camera (runs every frame)
	//
	// deltaTime - The time between frames
	// --------------------------------------------------------
	void Update(float deltaTime);

	// --------------------------------------------------------
	// Set the focus object for this camera
	// --------------------------------------------------------
	void SetFocusObj(GameObject* obj);

	// --------------------------------------------------------
	// Set the anchor point
	//
	// anchorPoint - the anchor position (minimum zoom)
	// --------------------------------------------------------
	void SetAnchor(DirectX::XMFLOAT3 anchor);

	// --------------------------------------------------------
	// Set the max zoom
	//
	// maxZoom - the max zoom distance to the focus object
	// --------------------------------------------------------
	void SetMaxZoom(float zoom);

	// --------------------------------------------------------
	// Set the anchor's rotation
	//
	// anchorRot - the anchor's rotation
	// --------------------------------------------------------
	void SetAnchorRot(DirectX::XMFLOAT3 anchorRotation);

	// --------------------------------------------------------
	// Set the y minimum for the camera
	//
	// yMinimum - the minimum y value
	// --------------------------------------------------------
	void SetYMin(float yMinimum);
};

