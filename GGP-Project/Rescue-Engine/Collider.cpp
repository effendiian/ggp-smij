#include "Collider.h"
#include "ResourceManager.h"

using namespace DirectX;

// Create an empty collider.
Collider::Collider(){}

// Create a collider from a position and size.
Collider::Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size)
	: Collider(position, size, DirectX::XMFLOAT3(0, 0, 0))
{}

// Create a collider from a position, size, and offset.
Collider::Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
{
	XMVECTOR center = XMLoadFloat3(&position);
	XMVECTOR off = XMLoadFloat3(&offset);
	XMStoreFloat3(&this->position, center + off);

	this->size = size;
	this->offset = offset;

	debug = false;

	box = ResourceManager::GetInstance()->GetMesh("Assets\\Models\\cube.obj");
}

// Release resources.
Collider::~Collider() {}

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
}

// Set the collider size.
void Collider::SetSize(DirectX::XMFLOAT3 newSize)
{
	XMVECTOR newDimensions = XMLoadFloat3(&newSize);
	XMStoreFloat3(&size, newDimensions);
}

// Check if a collision has occured.
bool Collider::Collides(Collider other)
{
	//JavaScript reference from MDN:
	//return (a.minX <= b.maxX && a.maxX >= b.minX) &&
	//	(a.minY <= b.maxY && a.maxY >= b.minY) &&
	//	(a.minZ <= b.maxZ && a.maxZ >= b.minZ);

	//AABB
	return (position.x - (size.x / 2) <= other.GetPosition().x + (other.GetSize().x) && position.x + (size.x / 2) >= other.GetPosition().x - (other.GetSize().x) &&
		position.y - (size.y / 2) <= other.GetPosition().y + (other.GetSize().y) && position.y + (size.y / 2) >= other.GetPosition().y - (other.GetSize().y) &&
		position.z - (size.z / 2) <= other.GetPosition().z + (other.GetSize().z) && position.z + (size.z / 2) >= other.GetPosition().z - (other.GetSize().z)
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

bool Collider::IsDebug()
{
	return debug;
}

void Collider::SetDebug(bool setting)
{
	debug = setting;
}

Mesh * Collider::getMesh()
{
	return box;
}
