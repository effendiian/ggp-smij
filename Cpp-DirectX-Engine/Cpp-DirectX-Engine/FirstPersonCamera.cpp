#include "FirstPersonCamera.h"

using namespace DirectX;

// Constructor - Set up the first person camera
FirstPersonCamera::FirstPersonCamera()
{
	inputManager = InputManager::GetInstance();
}

// Destructor for when an instance is deleted
FirstPersonCamera::~FirstPersonCamera()
{ }

// Update the camera (runs every frame)
void FirstPersonCamera::Update(float deltaTime, float moveSpeed)
{
	XMFLOAT3 movement = XMFLOAT3();

	//Relative Z movement
	if (inputManager->GetKey('W'))
	{
		movement.z += moveSpeed * deltaTime;
	}
	else if (inputManager->GetKey('S'))
	{
		movement.z -= moveSpeed * deltaTime;
	}

	//Relative X movement
	if (inputManager->GetKey('D'))
	{
		movement.x += moveSpeed * deltaTime;
	}
	else if (inputManager->GetKey('A'))
	{
		movement.x -= moveSpeed * deltaTime;
	}

	//Absolute Y movement
	if (inputManager->GetKey('Q'))
	{
		MoveAbsolute(XMFLOAT3(0, +moveSpeed * deltaTime, 0));
	}
	else if (inputManager->GetKey('E'))
	{
		MoveAbsolute(XMFLOAT3(0, -moveSpeed * deltaTime, 0));
	}

	// Rotate the movement vector
	XMVECTOR move = XMVector3Rotate(XMLoadFloat3(&movement),
		XMLoadFloat4(&GetQuatRotation()));
	//Add to position and store
	XMStoreFloat3(&movement, XMVectorAdd(XMLoadFloat3(&GetPosition()), move));
	SetPosition(movement);

	//Check if the right mouse button is held down
	if (inputManager->GetMouseButtonDown(MouseButtons::R))
	{
		SetCursorPos(inputManager->GetWindowCenterX(), inputManager->GetWindowCenterY());
	}
	else if (inputManager->GetMouseButton(MouseButtons::R))
	{
		CalculateCameraRotFromMouse();
	}

	Camera::Update(deltaTime);
}

// Calculate the camera's rotation when moving
void FirstPersonCamera::CalculateCameraRotFromMouse()
{
	float speed = 0.25f;

	//Get the center position of the window
	long centerX = inputManager->GetWindowCenterX();
	long centerY = inputManager->GetWindowCenterY();

	//Get the mouse position
	long mouseX = inputManager->GetMouseX();
	long mouseY = inputManager->GetMouseY();

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (mouseX > centerX)
	{
		fDeltaMouse = static_cast<float>(mouseX - centerX);
		fAngleY += fDeltaMouse * speed;
	}
	else if (mouseX < centerX)
	{
		fDeltaMouse = static_cast<float>(centerX - mouseX);
		fAngleY -= fDeltaMouse * speed;
	}

	if (mouseY > centerY)
	{
		fDeltaMouse = static_cast<float>(centerY - mouseY);
		fAngleX -= fDeltaMouse * speed;
	}
	else if (mouseY < centerY)
	{
		fDeltaMouse = static_cast<float>(mouseY - centerY);
		fAngleX += fDeltaMouse * speed;
	}

	static float xRot = 0;
	static float yRot = 0;

	xRot += fAngleX;
	yRot += fAngleY;

	//Keep camera from reversing when looking up/down
	if (xRot > 89.9f)
		xRot = 89.9f;
	if (xRot < -89.9f)
		xRot = -89.9f;

	//Change the Yaw and the Pitch of the camera
	SetRotation(xRot, yRot, GetRotation().z);
	SetCursorPos(centerX, centerY); //Position the mouse in the center
}