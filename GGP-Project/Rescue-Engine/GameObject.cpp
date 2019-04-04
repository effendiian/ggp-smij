#include "GameObject.h"

// For the DirectX Math library
using namespace DirectX;

// Constructor - Set up the gameobject.
GameObject::GameObject()
{
	//Set default transformation values
	Enable = true;
	world = XMFLOAT4X4();
	position = XMFLOAT3(0, 0, 0);
	SetRotation(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	worldDirty = false;
	RebuildWorld();
	
	collider = nullptr;
	//collider = new Collider(position, XMFLOAT3(0.5f, 0.5f, 0.5f));
}

GameObject::GameObject(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
	: GameObject()
{
	collider = new Collider(position, size, offset);
}

// Destructor for when an instance is deleted
GameObject::~GameObject()
{ 
	this->OnDestroy();
	if(collider != nullptr) delete collider;
}

void GameObject::Start()
{ }

void GameObject::Reset() 
{ }

void GameObject::OnDestroy() 
{ }

void GameObject::Update(float deltaTime)
{ }

// Get the world matrix for this GameObject (rebuilding if necessary)
XMFLOAT4X4 GameObject::GetWorldMatrix()
{
	//Rebuild the world if it is not current
	if (worldDirty)
		RebuildWorld();

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

XMFLOAT3 GameObject::GetForwardAxis()
{
	return forwardAxis;
}

// Get the rotation for this GameObject (Quaternion)
XMFLOAT3 GameObject::GetRotation()
{
	return rotation;
}

// Get the quaternion rotation for this entity (Quaternion)
DirectX::XMFLOAT4 GameObject::GetQuatRotation()
{
	return rotationQuat;
}

// Set the rotation for this GameObject (Quaternion)
void GameObject::SetRotation(XMFLOAT3 newRotation)
{
	worldDirty = true;
	rotation = newRotation;

	//Convert to quaternions and store
	XMVECTOR angles = XMVectorScale(XMLoadFloat3(&rotation), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);
	XMStoreFloat4(&rotationQuat, quat);

	//Rotate the forward axis
	XMStoreFloat3(&forwardAxis, XMVector3Normalize(
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quat)));
}

// Set the rotation for this GameObject using euler angles (Quaternion)
void GameObject::SetRotation(float x, float y, float z)
{
	worldDirty = true;

	//Convert to degrees
	rotation = XMFLOAT3(x, y, z);

	//Convert to quaternions and store
	XMVECTOR angles = XMVectorScale(XMLoadFloat3(&rotation), XM_PI / 180.0f);
	XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(angles);
	XMStoreFloat4(&rotationQuat, quat);

	//Rotate the forward axis
	XMStoreFloat3(&forwardAxis, XMVector3Normalize(
		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quat)));
}

// Rotate this GameObject (Angles)
void GameObject::Rotate(DirectX::XMFLOAT3 newRotation)
{
	XMFLOAT3 rot;
	XMStoreFloat3(&rot, XMLoadFloat3(&rotation) + XMLoadFloat3(&newRotation));
	SetRotation(rot);
}

// Rotate this GameObject using angles
void GameObject::Rotate(float x, float y, float z)
{
	XMFLOAT3 rot;
	XMStoreFloat3(&rot, XMLoadFloat3(&rotation) + XMVectorSet(x, y,z, 0));
	SetRotation(rot);
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

Collider * GameObject::GetCollider()
{
	return collider;
}

void GameObject::AddCollider(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
{
	if (collider == nullptr)
	{
		collider = new Collider(position, size, offset);
	}
}
