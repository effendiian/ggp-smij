#include "Collider.h"
#include "ResourceManager.h"

using namespace DirectX;

// Create an empty collider.
Collider::Collider(XMFLOAT3 position)
{
	this->position = position;
	this->rotation = XMFLOAT4(0, 0, 0, 0);
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

	this->rotation = XMFLOAT4(0, 0, 0, 0);

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

	//Get rotation matrix
	XMMATRIX rotating = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));

	//Get scale matrix
	XMMATRIX scaling = XMMatrixScalingFromVector(XMLoadFloat3(&size));

	//Calculate matrix and store
	XMMATRIX newWorld = XMMatrixTranspose(scaling * rotating * translation);
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

DirectX::XMVECTOR Collider::GetNormal(DirectX::XMFLOAT4 axis)
{
	//return worldMatrix * axis
	XMMATRIX world = XMLoadFloat4x4(&GetWorldMatrix());
	XMVECTOR direction = XMLoadFloat4(&axis);

	return XMVector4Transform(direction, world);

	//XMFLOAT3 normal;
	//XMStoreFloat3(&normal, XMVector4Transform(direction, world));
	//return normal;
}

DirectX::XMVECTOR Collider::GetCenterGlobal()
{
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	XMVECTOR centerLocal = XMLoadFloat3(&position);
	return XMVector4Transform(centerLocal, world);
}

// Set the collider position.
void Collider::SetPosition(DirectX::XMFLOAT3 newPosition)
{
	XMVECTOR newPos = XMLoadFloat3(&newPosition);
	XMVECTOR off = XMLoadFloat3(&offset);
	XMStoreFloat3(&position, XMVectorAdd(newPos, off));
	worldDirty = true;
}

void Collider::SetRotation(DirectX::XMFLOAT4 newRotation)
{
	rotation = newRotation;
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
bool Collider::Collides(Collider* other)
{
	//JavaScript reference from MDN:
	//return (a.minX <= b.maxX && a.maxX >= b.minX) &&
	//	(a.minY <= b.maxY && a.maxY >= b.minY) &&
	//	(a.minZ <= b.maxZ && a.maxZ >= b.minZ);

	//AABB
	//Get half sizes
	/*XMFLOAT3 otherPosition = other.GetPosition();
	XMFLOAT3 size = GetHalfSize();
	XMFLOAT3 otherSize = other.GetHalfSize();*/

	/*return (position.x - size.x <= otherPosition.x + otherSize.x && position.x + size.x >= otherPosition.x - otherSize.x &&
		position.y - size.y <= otherPosition.y + otherSize.y && position.y + size.y >= otherPosition.y - otherSize.y &&
		position.z - size.z <= otherPosition.z + otherSize.z && position.z + size.z >= otherPosition.z - otherSize.z
		);*/

	return SAT(other);

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

bool Collider::SAT(Collider* other)
{
	//SAT
	//variables for switching between axes to check
	float ra;
	float rb;

	XMFLOAT3X3 rotAinB; //rotation matrix expressing rigidbody B (other) in a's vector space
	XMFLOAT3X3 subexpressions; //absolute value of rotAinB plus epsilon constant

	//xyz axes for each rigidbody
	XMVECTOR axesA[3];
	XMVECTOR axesB[3];

	//Populates axis arrays using helper functions
	axesA[0] = GetNormal(XMFLOAT4(1, 0, 0, 0)); //x
	axesA[1] = GetNormal(XMFLOAT4(0, 1, 0, 0)); //y
	axesA[2] = GetNormal(XMFLOAT4(0, 0, 1, 0)); //z
	axesB[0] = other->GetNormal(XMFLOAT4(1, 0, 0, 0)); //x
	axesB[1] = other->GetNormal(XMFLOAT4(0, 1, 0, 0)); //y
	axesB[2] = other->GetNormal(XMFLOAT4(0, 0, 1, 0)); //z

	//Populates rotAinB
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rotAinB.m[i][j] = XMVectorGetX(XMVector3Dot(axesA[i], axesB[j]));

	//Vector between rigidbodies
	XMVECTOR translation = XMLoadFloat3(&other->GetPosition()) - XMLoadFloat3(&GetPosition());
	//Converted into A's vector space
	float tx = XMVectorGetX(XMVector3Dot(translation, axesA[0]));
	float ty = XMVectorGetX(XMVector3Dot(translation, axesA[1]));
	float tz = XMVectorGetX(XMVector3Dot(translation, axesA[2]));
	translation = XMLoadFloat3(&XMFLOAT3(tx, ty, tz));

	//Populates subexpressions
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			subexpressions.m[i][j] = abs(rotAinB.m[i][j]) + std::numeric_limits<float>::epsilon();

	XMVECTOR thisHalf = XMLoadFloat3(&GetHalfSize()); //half size of this collider
	XMVECTOR otherHalf = XMLoadFloat3(&other->GetHalfSize()); //half size of other collider

	//Checks first three axes (A's xyz)
	for (int i = 0; i < 3; i++)
	{
		ra = XMVectorGetByIndex(thisHalf, i);
		rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[i][0] + XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[i][1] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[i][2];
		if (abs(XMVectorGetByIndex(translation, i)) > ra + rb) return false;
	}

	//Checks next three axes (B's xyz)
	for (int i = 0; i < 3; i++)
	{
		ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[0][i] + XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[1][i] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[2][i];
		rb = XMVectorGetByIndex(otherHalf, i);
		if (abs(XMVectorGetByIndex(translation, 0) * rotAinB.m[0][i] + XMVectorGetByIndex(translation, 1) * rotAinB.m[1][i] + XMVectorGetByIndex(translation, 2) * rotAinB.m[2][i]) > ra + rb) return false;
	}

	//Tests nine more axes
	//Each axis is the crossproduct of a combination
	//of two axes from the first six tests

	// Test axis L = A0 x B0
	ra = XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[2][0] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[1][0];
	rb = XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[0][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[0][1];
	if (abs(XMVectorGetByIndex(translation, 2) * rotAinB.m[1][0] - XMVectorGetByIndex(translation, 1) * rotAinB.m[2][0]) > ra + rb) return false;
	// Test axis L = A0 x B1
	ra = XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[2][1] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[1][1];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[0][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[0][0];
	if (abs(XMVectorGetByIndex(translation, 2) * rotAinB.m[1][1] - XMVectorGetByIndex(translation, 1) * rotAinB.m[2][1]) > ra + rb) return false;
	// Test axis L = A0 x B2
	ra = XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[2][2] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[1][2];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[0][1] + XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[0][0];
	if (abs(XMVectorGetByIndex(translation, 2) * rotAinB.m[1][2] - XMVectorGetByIndex(translation, 1) * rotAinB.m[2][2]) > ra + rb) return false;
	// Test axis L = A1 x B0
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[2][0] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[0][0];
	rb = XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[1][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[1][1];
	if (abs(XMVectorGetByIndex(translation, 0) * rotAinB.m[2][0] - XMVectorGetByIndex(translation, 2) * rotAinB.m[0][0]) > ra + rb) return false;
	// Test axis L = A1 x B1
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[2][1] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[0][1];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[1][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[1][0];
	if (abs(XMVectorGetByIndex(translation, 0) * rotAinB.m[2][1] - XMVectorGetByIndex(translation, 2) * rotAinB.m[0][1]) > ra + rb) return false;
	// Test axis L = A1 x B2
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[2][2] + XMVectorGetByIndex(thisHalf, 2) * subexpressions.m[0][2];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[1][1] + XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[1][0];
	if (abs(XMVectorGetByIndex(translation, 0) * rotAinB.m[2][2] - XMVectorGetByIndex(translation, 2) * rotAinB.m[0][2]) > ra + rb) return false;
	// Test axis L = A2 x B0
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[1][0] + XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[0][0];
	rb = XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[2][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[2][1];
	if (abs(XMVectorGetByIndex(translation, 1) * rotAinB.m[0][0] - XMVectorGetByIndex(translation, 0) * rotAinB.m[1][0]) > ra + rb) return false;
	// Test axis L = A2 x B1
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[1][1] + XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[0][1];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[2][2] + XMVectorGetByIndex(otherHalf, 2) * subexpressions.m[2][0];
	if (abs(XMVectorGetByIndex(translation, 1) * rotAinB.m[0][1] - XMVectorGetByIndex(translation, 0) * rotAinB.m[1][1]) > ra + rb) return false;
	// Test axis L = A2 x B2
	ra = XMVectorGetByIndex(thisHalf, 0) * subexpressions.m[1][2] + XMVectorGetByIndex(thisHalf, 1) * subexpressions.m[0][2];
	rb = XMVectorGetByIndex(otherHalf, 0) * subexpressions.m[2][1] + XMVectorGetByIndex(otherHalf, 1) * subexpressions.m[2][0];
	if (abs(XMVectorGetByIndex(translation, 1) * rotAinB.m[0][2] - XMVectorGetByIndex(translation, 0) * rotAinB.m[1][2]) > ra + rb) return false;

	return true;
}
