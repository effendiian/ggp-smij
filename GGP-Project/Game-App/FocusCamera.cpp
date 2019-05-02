#include "FocusCamera.h"
#include "ExtendedMath.h"

using namespace DirectX;

// Constructor - Set up the focus camera
FocusCamera::FocusCamera(GameObject* obj, DirectX::XMFLOAT3 anchor,
	XMFLOAT3 anchorRotation, float zoomDist, float yMinimum) : Camera()
{
	inputManager = InputManager::GetInstance();
	focusObj = obj;
	anchorPos = anchor;
	maxZoom = zoomDist;
	zoom = 0;
	zoomTick = 0.1f;
	yMin = yMinimum;
	moveIn = false;

	//Initial position and rot
	SetPosition(anchor);
	SetAnchorRot(anchorRotation);

#if defined(DEBUG) || defined(_DEBUG)
	prevKeypress = false;
	printf("Debug camera enabled. Press 'f' to toggle between focus and fps movement\n");
#endif
}

// Destructor for when an instance is deleted
FocusCamera::~FocusCamera()
{ }

// Update the camera (runs every frame)
void FocusCamera::Update(float deltaTime)
{
#if defined(DEBUG) || defined(_DEBUG)
	//User is allows to press 'f' to toggle to the debug camera if in debug mode
	bool keypress = inputManager->GetKey('F');
	if (keypress && !prevKeypress)
	{
		fpsMovement = !fpsMovement;
	}
	prevKeypress = keypress;

	//Debug camera selection
	if (fpsMovement)
	{
		FPSMovement(deltaTime);
		Camera::Update(deltaTime);
		return;
	}
#endif

	//Get scroll delta and apply it to zoom
	float scrollDelta = inputManager->GetScrollWheelDelta();
	zoom += scrollDelta * zoomTick;
	if (scrollDelta > 0)
		moveIn = true;
	else if (scrollDelta < 0)
		moveIn = false;

	//Keep zoom inbounds
	zoom = ExtendedMath::Clamp(zoom, 0.0f, 1.0f);
	
	//Get target position
	//XMFLOAT3 targetPos;
	XMVECTOR targetPos = XMVectorLerp(XMLoadFloat3(&anchorPos),
		XMLoadFloat3(&focusObj->GetPosition()), zoom);

	//Lerp new position
	XMVECTOR newPos = XMVectorLerp(XMLoadFloat3(&GetPosition()), targetPos, 0.005f);

	//Check if we are too close
	XMFLOAT3 position;
	XMVECTOR distVec = XMVector3Length(XMVectorSubtract(XMLoadFloat3(&focusObj->GetPosition()), newPos));
	float distance = XMVectorGetX(distVec);
	if (moveIn && distance <= maxZoom)
	{
		position = GetPosition();
	}
	else XMStoreFloat3(&position, newPos);

	if (position.y < yMin)
		position.y = yMin;

	//Set new position
	SetPosition(position);

	//Rotate camera
	XMFLOAT4X4 rotMat;
	XMStoreFloat4x4(&rotMat, XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()),
		XMLoadFloat3(&focusObj->GetPosition()),
		XMLoadFloat3(&GetUpAxis())));

	XMFLOAT4 destRot = ExtendedMath::MatrixToQuaternion(rotMat);
	SetRotation(destRot);

	Camera::Update(deltaTime);
}

// Set the focus object for this camera
void FocusCamera::SetFocusObj(GameObject* obj)
{
	focusObj = obj;
}

// Set the anchor point
void FocusCamera::SetAnchor(XMFLOAT3 anchor)
{
	anchorPos = anchor;
}

// Set the max zoom
void FocusCamera::SetMaxZoom(float zoom)
{
	maxZoom = zoom;
}

// Set the anchor's rotation
void FocusCamera::SetAnchorRot(DirectX::XMFLOAT3 anchorRotation)
{
	SetRotation(anchorRotation);
	anchorRot = GetRotation();
}

// Set the y minimum for the camera
void FocusCamera::SetYMin(float yMinimum)
{
	yMin = yMinimum;
}

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
// Allow fps movement for debugging
void FocusCamera::FPSMovement(float deltaTime)
{

//Allow FPS movement in debug mode
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
		XMLoadFloat4(&GetRotation()));
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
void FocusCamera::CalculateCameraRotFromMouse()
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
	SetRotation(xRot, yRot, 0);
	SetCursorPos(centerX, centerY); //Position the mouse in the center
}
#endif
