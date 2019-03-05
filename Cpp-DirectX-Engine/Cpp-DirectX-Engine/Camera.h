#pragma once
#include <DirectXMath.h>
#include "GameObject.h"

// --------------------------------------------------------
// A camera definition.
// --------------------------------------------------------
class Camera : public GameObject
{
private:
	//Matrices
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	//Transformation data
	DirectX::XMFLOAT3 up;

public:
	// --------------------------------------------------------
	// Constructor - Set up the camera
	// (Remember to create the projection matrix right after!)
	// --------------------------------------------------------
	Camera();

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~Camera();

	// --------------------------------------------------------
	// Update the camera (runs every frame)
	//
	// deltaTime - The time between frames
	// --------------------------------------------------------
	virtual void Update(float deltaTime);

	// --------------------------------------------------------
	// Create the camera's view matrix from fields
	// --------------------------------------------------------
	void CreateViewMatrix();

	// --------------------------------------------------------
	// Get the camera's view matrix
	// --------------------------------------------------------
	DirectX::XMFLOAT4X4 GetViewMatrix();

	// --------------------------------------------------------
	// Create the camera's projection matrix from parameters
	//
	// fov - FOV of the camera
	// aspectRatio - Aspect ratio of the camera (width / height)
	// nearClip - Near clip plane distance
	// farClip - Far clip plane distance
	// --------------------------------------------------------
	void CreateProjectionMatrix(float fov, float aspectRatio, 
									float nearClip, float farClip);

	// --------------------------------------------------------
	// Get the camera's projection matrix
	// --------------------------------------------------------
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
};

