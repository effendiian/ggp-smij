#pragma once
#include <DirectXMath.h>

class Collider
{
private:
	//Transform vars
	DirectX::XMFLOAT3 position; //center
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT3 offset; //(0,0,0) if not given
	DirectX::XMFLOAT3 size; //xyz = width, height, length

	//World matrices
	bool worldDirty;
	DirectX::XMFLOAT4X4 worldMatrix;

	// --------------------------------------------------------
	// Construct this collider's world matrix
	// --------------------------------------------------------
	void ConstructWorldMatrix();

public:
	//Constructors

	// --------------------------------------------------------
	// Construct a collider with default values
	//
	// position - position of the gameobject this collider is attatched to
	// --------------------------------------------------------
	Collider(DirectX::XMFLOAT3 position);

	// --------------------------------------------------------
	// Construct a collider
	//
	// position - position of the gameobject this collider is attatched to
	// size - size of the collider
	// --------------------------------------------------------
	Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size);

	// --------------------------------------------------------
	// Construct a collider
	//
	// position - position of the gameobject this collider is attatched to
	// size - size of the collider
	// offset - the offset from the center of the gameobject this collider is on
	// --------------------------------------------------------
	Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset); //optional center offset

	~Collider();

	// --------------------------------------------------------
	// Get this collider's world matrix
	// --------------------------------------------------------
	DirectX::XMFLOAT4X4 GetWorldMatrix();

	// --------------------------------------------------------
	// Get the position of the collider
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetPosition() const;

	// --------------------------------------------------------
	// Get the size of the collider
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetSize() const;

	// --------------------------------------------------------
	// Get the half size of the collider
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetHalfSize() const;

	DirectX::XMVECTOR GetNormal(DirectX::XMFLOAT4 axis);

	DirectX::XMVECTOR GetCenterGlobal();

	// --------------------------------------------------------
	// Set the position of the collider
	// --------------------------------------------------------
	void SetPosition(DirectX::XMFLOAT3 newPosition);

	// --------------------------------------------------------
	// Set the rotation of the collider (quaternion)
	// --------------------------------------------------------
	void SetRotation(DirectX::XMFLOAT4 newRotation);

	// --------------------------------------------------------
	// Set the size of the collider
	// --------------------------------------------------------
	void SetSize(DirectX::XMFLOAT3 newSize);

	// --------------------------------------------------------
	// Check if the collider collides with another (AABB)
	// --------------------------------------------------------
	bool Collides(Collider* other); //AABB for now

	bool SAT(Collider* other);


};

