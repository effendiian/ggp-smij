#include "Collider.h"
#include "ResourceManager.h"

using namespace DirectX;

// Create an empty collider.
Collider::Collider(XMFLOAT3 position)
{
	this->position = position;
	this->size = XMFLOAT3();
	this->offset = XMFLOAT3();
	this->worldMatrix = XMFLOAT4X4();

	worldDirty = true;
}

// Create a collider from a position and size.
Collider::Collider(XMFLOAT3 position, XMFLOAT3 size)
	: Collider(position, size, XMFLOAT3(0, 0, 0))
{ }

// Create a collider from a position, size, and offset.
Collider::Collider(XMFLOAT3 position, XMFLOAT3 size, XMFLOAT3 offset)
{
	XMStoreFloat3(&this->position, XMLoadFloat3(&position) + XMLoadFloat3(&offset));

	this->size = size;
	this->offset = offset;
	this->worldMatrix = XMFLOAT4X4();

	worldDirty = true;
}

// Release resources.
Collider::~Collider() {}

// Construct this collider's world matrix
void Collider::ConstructWorldMatrix()
{
	//Get translation matrix
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));

	//Get scale matrix
	XMMATRIX scaling = XMMatrixScalingFromVector(XMLoadFloat3(&size));

	//Calculate matrix and store
	XMMATRIX newWorld = XMMatrixTranspose(scaling * translation);
	XMStoreFloat4x4(&worldMatrix, newWorld);

	worldDirty = false;
}


// Get this collider's world matrix
DirectX::XMFLOAT4X4 Collider::GetWorldMatrix()
{
	if (worldDirty)
		ConstructWorldMatrix();

	return worldMatrix;
}

// Return the position of the collider.
DirectX::XMFLOAT3 Collider::GetPosition() const
{
	return position;
}

// Return dimensions.
DirectX::XMFLOAT3 Collider::GetSize() const
{
	return size;
}

// Return half size.
DirectX::XMFLOAT3 Collider::GetHalfSize() const 
{
	return DirectX::XMFLOAT3(size.x / 2, size.y / 2, size.z / 2);
}

// Set the collider position.
void Collider::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	XMVECTOR newPos = XMLoadFloat3(&newPosition);
	XMVECTOR off = XMLoadFloat3(&offset);
	XMStoreFloat3(&position, newPos + off);
	worldDirty = true;
}

// Set the collider size.
void Collider::SetSize(DirectX::XMFLOAT3 newSize)
{
	XMVECTOR newDimensions = XMLoadFloat3(&newSize);
	XMStoreFloat3(&size, newDimensions);
	worldDirty = true;
}

// Check if a collision has occured.
bool Collider::Collides(Collider other)
{
	//JavaScript reference from MDN:
	//return (a.minX <= b.maxX && a.maxX >= b.minX) &&
	//	(a.minY <= b.maxY && a.maxY >= b.minY) &&
	//	(a.minZ <= b.maxZ && a.maxZ >= b.minZ);

	//Get half sizes
	XMFLOAT3 otherPosition = other.GetPosition();
	XMFLOAT3 size = GetHalfSize();
	XMFLOAT3 otherSize = other.GetHalfSize();

	//AABB
	return (position.x - size.x <= otherPosition.x + otherSize.x && position.x + size.x >= otherPosition.x - otherSize.x &&
		position.y - size.y <= otherPosition.y + otherSize.y && position.y + size.y >= otherPosition.y - otherSize.y &&
		position.z - size.z <= otherPosition.z + otherSize.z && position.z + size.z >= otherPosition.z - otherSize.z
		);

	//Circle Collision
	/*XMVECTOR thisPos = XMLoadFloat3(&position);
	XMVECTOR otherPos = XMLoadFloat3(&other.GetPosition());

	XMFLOAT3 vDistance;
	XMStoreFloat3(&vDistance, otherPos - thisPos);

	float distance = (vDistance.x * vDistance.x) + (vDistance.y * vDistance.y) + (vDistance.z * vDistance.z);

	distance = sqrtf(distance);

	if (distance < 0.5f) return true;
	else return false;*/
	
}
