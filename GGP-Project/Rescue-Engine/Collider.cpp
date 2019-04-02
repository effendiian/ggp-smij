#include "Collider.h"

using namespace DirectX;

Collider::Collider()
{
}

Collider::Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size)
{
	this->position = position;
	this->size = size;
	this->offset = XMFLOAT3(0, 0, 0);
}

Collider::Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset)
{
	XMVECTOR center = XMLoadFloat3(&position);
	XMVECTOR off = XMLoadFloat3(&offset);
	XMStoreFloat3(&this->position, center + off);

	this->size = size;
	this->offset = offset;
}


Collider::~Collider()
{
}

DirectX::XMFLOAT3 Collider::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Collider::GetSize()
{
	return size;
}

void Collider::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	XMVECTOR newPos = XMLoadFloat3(&newPosition);
	XMVECTOR off = XMLoadFloat3(&offset);
	XMStoreFloat3(&position, newPos + off);
}

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
