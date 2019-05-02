#include "GameObject.h"
#include "Renderer.h"

// For the DirectX Math library
using namespace DirectX;

// Constructor - Set up the gameobject.
GameObject::GameObject()
{
	//Set default transformation values
	world = XMFLOAT4X4();
	position = XMFLOAT3(0, 0, 0);
	collider = nullptr;
	SetRotation(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	worldDirty = false;
	RebuildWorld();
	debug = false;

	enabled = true;
	name = "GameObject";
}

// Constructor - Set up the gameobject.
GameObject::GameObject(std::string name)
	: GameObject()
{
	this->name = name;
}

// Destructor for when an instance is deleted
GameObject::~GameObject()
{ 
	if(collider != nullptr) delete collider;
}

// Get the enabled state of the gameobject
bool GameObject::GetEnabled()
{
	return enabled;
}

// Enable or disable the gameobject
void GameObject::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}

// Set the name of this gameobject
void GameObject::SetName(std::string name)
{
	this->name = name;
}

// Get the name of this gameobject
std::string GameObject::GetName()
{
	return name;
}

// Update this entity
void GameObject::Update(float deltaTime)
{ }

// Get the world matrix for this GameObject (rebuilding if necessary)
XMFLOAT4X4 GameObject::GetWorldMatrix()
{
	//Rebuild the world if it is not current
	if (worldDirty)
		RebuildWorld();

	//Add collider to render list
	if (collider != nullptr && IsDebug())
		Renderer::GetInstance()->AddDebugCubeToThisFrame(collider->GetWorldMatrix());

	return world;
}

// Get the inverse transpose of the world matrix for this entity (rebuilding if necessary)
XMFLOAT4X4 GameObject::GetWorldInvTransMatrix()
{
	//Rebuild the world if it is not current
	if (worldDirty)
		RebuildWorld();

	return worldInvTrans;
}

// Rebuild the world matrix from the different components
void GameObject::RebuildWorld()
{
	//Get translation matrix
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));

	//Get rotation matrix
	XMMATRIX rotating = XMMatrixRotationQuaternion(XMLoadFloat4(&rotationQuat));

	//Get scale matrix
	XMMATRIX scaling = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	//Calculate matrix and store
	XMMATRIX newWorld = XMMatrixTranspose(scaling * rotating * translation);
	XMStoreFloat4x4(&world, newWorld);

	//Calculate inverse transpose
	XMStoreFloat4x4(&worldInvTrans, XMMatrixInverse(&XMVectorSet(-1, -1, -1, -1), XMMatrixTranspose(newWorld)));

	worldDirty = false;
}

// Get the position for this GameObject
XMFLOAT3 GameObject::GetPosition()
{
	return position;
}

// Set the position for this GameObject
void GameObject::SetPosition(XMFLOAT3 newPosition)
{
	worldDirty = true;
	position = newPosition;
	if (collider != nullptr) collider->SetPosition(position);
}

// Set the position for this GameObject
void GameObject::SetPosition(float x, float y, float z)
{
	worldDirty = true;

	//Set values
	position = XMFLOAT3(x, y, z);
	if (collider != nullptr) collider->SetPosition(position);
}

// Moves this GameObject in absolute space by a given vector.
// Does not take rotation into account
void GameObject::MoveAbsolute(XMFLOAT3 moveAmnt)
{
	worldDirty = true;

	//Add the vector to the position
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position),
		XMLoadFloat3(&moveAmnt)));
	if (collider != nullptr) collider->SetPosition(position);
}

// Moves this GameObject in relative space by a given vector.
// Does take rotation into account
void GameObject::MoveRelative(XMFLOAT3 moveAmnt)
{
	worldDirty = true;

	// Rotate the movement vector
	XMVECTOR move = XMVector3Rotate(XMLoadFloat3(&moveAmnt),
		XMLoadFloat4(&rotationQuat));

	//Add to position and
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), move));
	if (collider != nullptr) collider->SetPosition(position);
}

// Get the rotated forward axis of this gameobject
XMFLOAT3 GameObject::GetForwardAxis()
{
	return forwardAxis;
}

// Get the rotated right axis of this gameobject
XMFLOAT3 GameObject::GetRightAxis()
{
	return rightAxis;
}

// Get the rotated up axis of this gameobject
XMFLOAT3 GameObject::GetUpAxis()
{
	return upAxis;
}

// Get the quaternion rotation for this entity (Quaternion)
DirectX::XMFLOAT4 GameObject::GetRotation()
{
	return rotationQuat;
}

// Set the rotation for this GameObject (Quaternion)
void GameObject::SetRotation(XMFLOAT3 newRotation)
{
	worldDirty = true;

	//Convert to quaternions and store
	XMVECTOR angles = XMVectorScale(XMLoadFloat3(&newRotation), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);
	XMStoreFloat4(&rotationQuat, quat);

	CalculateAxis();

	//Apply to collider
	if (collider != nullptr) collider->SetRotation(rotationQuat);
}

// Set the rotation for this GameObject using euler angles (Quaternion)
void GameObject::SetRotation(float x, float y, float z)
{
	worldDirty = true;

	//Convert to quaternions and store
	XMVECTOR angles = XMVectorScale(XMVectorSet(x, y, z, 0), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);
	XMStoreFloat4(&rotationQuat, quat);

	CalculateAxis();

	//Apply to collider
	if (collider != nullptr) collider->SetRotation(rotationQuat);
}

// Set the rotation for this GameObject (Quaternion)
void GameObject::SetRotation(DirectX::XMFLOAT4 newQuatRotation)
{
	worldDirty = true;
	rotationQuat = newQuatRotation;

	CalculateAxis();

	//Apply to collider
	if (collider != nullptr) collider->SetRotation(rotationQuat);
}

// Rotate this GameObject (Angles)
void GameObject::Rotate(DirectX::XMFLOAT3 newRotation)
{
	XMVECTOR angles = XMVectorScale(XMLoadFloat3(&newRotation), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);

	XMFLOAT4 rot;
	XMStoreFloat4(&rot, XMQuaternionMultiply(XMLoadFloat4(&rotationQuat), quat));
	SetRotation(rot);
}

// Rotate this GameObject using angles
void GameObject::Rotate(float x, float y, float z)
{
	XMVECTOR angles = XMVectorScale(XMVectorSet(x, y, z, 0), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);

	XMFLOAT4 rot;
	XMStoreFloat4(&rot, XMQuaternionMultiply(XMLoadFloat4(&rotationQuat), quat));
	SetRotation(rot);
}

// Calculate the local axis for the gameobject
void GameObject::CalculateAxis()
{
	//Rotate the forward axis
	XMVECTOR forward = XMVector3Normalize(
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotationQuat)));
	XMStoreFloat3(&forwardAxis, forward);

	//Rotate the right axis
	XMVECTOR right = XMVector3Normalize(
		XMVector3Rotate(XMVectorSet(1, 0, 0, 0), XMLoadFloat4(&rotationQuat)));
	XMStoreFloat3(&rightAxis, right);

	//Rotate the up axis
	XMVECTOR up = XMVector3Normalize(
		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&rotationQuat)));
	XMStoreFloat3(&upAxis, up);
}

// Get the scale for this GameObject
XMFLOAT3 GameObject::GetScale()
{
	return scale;
}

// Set the scale for this GameObject
void GameObject::SetScale(XMFLOAT3 newScale)
{
	worldDirty = true;
	scale = newScale;
}

// Set the scale for this GameObject
void GameObject::SetScale(float x, float y, float z)
{
	worldDirty = true;

	//Set values
	scale = XMFLOAT3(x, y, z);
}

// Get this object's collider
Collider* GameObject::GetCollider()
{
	return collider;
}

// Add a collider to this object if it has none
void GameObject::AddCollider(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
{
	if (collider == nullptr)
	{
		collider = new Collider(position, size, offset);
	}
}

// Check if the collider is in debug mode (draw outline)
bool GameObject::IsDebug()
{
	return debug;
}

// Set debug mode for this collider (draw outline)
void GameObject::SetDebug(bool setting)
{
	debug = setting;
}
