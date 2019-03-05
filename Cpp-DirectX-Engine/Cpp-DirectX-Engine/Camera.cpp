#include "Camera.h"

using namespace DirectX;

// Constructor - Set up the camera
Camera::Camera()
{
	//Default transformation values
	up = XMFLOAT3(0, 1, 0);
	CreateViewMatrix();
}

// Destructor for when an instance is deleted
Camera::~Camera()
{ }

// Update the camera (runs every frame)
void Camera::Update(float deltaTime)
{
	CreateViewMatrix();
}

// Create the camera's view matrix from fields
void Camera::CreateViewMatrix()
{
	//Rotate the forward vector
	XMVECTOR forward = XMLoadFloat3(&GetForwardAxis());

	//Create the up vector from the forward
	XMVECTOR up = XMVector3Cross(XMVector3Cross(forward, XMLoadFloat3(&(this->up))), forward);

	//Create view matrix (transpose for HLSL)
	XMStoreFloat4x4(&view, XMMatrixTranspose(
		XMMatrixLookToLH(XMLoadFloat3(&GetPosition()), forward, up)));
}

// Get the camera's view matrix
XMFLOAT4X4 Camera::GetViewMatrix()
{
	return view;
}

// Create the camera's projection matrix from parameters
void Camera::CreateProjectionMatrix(float fov, float aspectRatio, 
										float nearClip, float farClip)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		fov,			// Field of View Angle
		aspectRatio,	// Aspect ratio
		nearClip,		// Near clip plane distance
		farClip);		// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// Get the camera's projection matrix
XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projection;
}