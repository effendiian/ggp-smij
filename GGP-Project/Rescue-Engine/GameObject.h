#pragma once
#include <DirectXMath.h>
#include "Collider.h"
#include <string>

// --------------------------------------------------------
// A GameObject definition.
//
// An GameObject contains world data
// --------------------------------------------------------
class GameObject
{
private:
	//Transformations
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 worldInvTrans;


	DirectX::XMFLOAT3 forwardAxis;
	DirectX::XMFLOAT3 rightAxis;
	DirectX::XMFLOAT3 upAxis;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotationQuat;
	DirectX::XMFLOAT3 scale;
	bool worldDirty;
	bool debug;

	//Other data
	Collider* collider;

	// --------------------------------------------------------
	// Calculate the local axis for the gameobject
	// --------------------------------------------------------
	void CalculateAxis();

protected:
	bool enabled;
	std::string name;

public:
	// --------------------------------------------------------
	// Constructor - Set up the gameobject.
	// --------------------------------------------------------
	GameObject();

	// --------------------------------------------------------
	// Constructor - Set up the gameobject.
	//
	// name - the name of the gameobject
	// --------------------------------------------------------
	GameObject(std::string name);

	// --------------------------------------------------------
	// Collider Constructor - Set up the gameobject with a collider
	//
	// size - dimensions of bounding box
	// offset - offset of collider from position of game object
	// --------------------------------------------------------
	/*GameObject(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset);*/

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	virtual ~GameObject();

	// --------------------------------------------------------
	// Get the enabled state of the gameobject
	// Disabled objects are not updated or drawn
	// --------------------------------------------------------
	bool GetEnabled();

	// --------------------------------------------------------
	// Enable or disable the gameobject
	// Disabled objects are not updated or drawn
	// --------------------------------------------------------
	void SetEnabled(bool enabled);

	// --------------------------------------------------------
	// Set the name of this gameobject
	// --------------------------------------------------------
	void SetName(std::string name);

	// --------------------------------------------------------
	// Get the name of this gameobject
	// --------------------------------------------------------
	std::string GetName();

	// --------------------------------------------------------
	// Update this entity
	// --------------------------------------------------------
	virtual void Update(float deltaTime);

	// --------------------------------------------------------
	// Get the world matrix for this GameObject (rebuilding if necessary)
	// --------------------------------------------------------
	DirectX::XMFLOAT4X4 GetWorldMatrix();

	// --------------------------------------------------------
	// Get the inverse transpose of the world matrix for this GameObject 
	// (rebuilding if necessary)
	// --------------------------------------------------------
	DirectX::XMFLOAT4X4 GetWorldInvTransMatrix();

	// --------------------------------------------------------
	// Rebuild the world matrix from the different components
	// --------------------------------------------------------
	void RebuildWorld();

	// --------------------------------------------------------
	// Get the position for this GameObject
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetPosition();

	// --------------------------------------------------------
	// Set the position for this GameObject
	//
	// newPosition - The new position to go to
	// --------------------------------------------------------
	void SetPosition(DirectX::XMFLOAT3 newPosition);

	// --------------------------------------------------------
	// Set the position for this GameObject
	//
	// x - new x position
	// y - new y position
	// z - new z position
	// --------------------------------------------------------
	void SetPosition(float x, float y, float z);

	// --------------------------------------------------------
	// Moves this GameObject in absolute space by a given vector.
	// Does not take rotation into account
	//
	// moveTo - The amount to move for each axis
	// --------------------------------------------------------
	void MoveAbsolute(DirectX::XMFLOAT3 moveAmnt);

	// --------------------------------------------------------
	// Moves this GameObject in relative space by a given vector.
	// Does take rotation into account
	//
	// moveTo - The amount to move for each axis
	// --------------------------------------------------------
	void MoveRelative(DirectX::XMFLOAT3 moveAmnt);

	// --------------------------------------------------------
	// Get the rotated forward axis of this gameobject
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetForwardAxis();

	// --------------------------------------------------------
	// Get the rotated right axis of this gameobject
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetRightAxis();

	// --------------------------------------------------------
	// Get the rotated up axis of this gameobject
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetUpAxis();

	// --------------------------------------------------------
	// Get the rotation for this GameObject (Quaternion)
	// --------------------------------------------------------
	DirectX::XMFLOAT4 GetRotation();

	// --------------------------------------------------------
	// Set the rotation for this GameObject (Angles)
	//
	// newRotation - The new rotation to rotate to
	// --------------------------------------------------------
	void SetRotation(DirectX::XMFLOAT3 newRotation);

	// --------------------------------------------------------
	// Set the rotation for this GameObject using angles
	//
	// x - x angle
	// y - y angle
	// z - z angle
	// --------------------------------------------------------
	void SetRotation(float x, float y, float z);

	// --------------------------------------------------------
	// Set the rotation for this GameObject (Quaternion)
	//
	// newQuatRotation - The new rotation to rotate to
	// --------------------------------------------------------
	void SetRotation(DirectX::XMFLOAT4 newQuatRotation);

	// --------------------------------------------------------
	// Rotate this GameObject (Angles)
	//
	// newRotation - The vector rotation
	// --------------------------------------------------------
	void Rotate(DirectX::XMFLOAT3 newRotation);

	// --------------------------------------------------------
	// Rotate this GameObject using angles
	//
	// x - x angle
	// y - y angle
	// z - z angle
	// --------------------------------------------------------
	void Rotate(float x, float y, float z);

	// --------------------------------------------------------
	// Get the scale for this GameObject
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetScale();

	// --------------------------------------------------------
	// Set the scale for this GameObject
	//
	// newScale - The new scale to resize to
	// --------------------------------------------------------
	void SetScale(DirectX::XMFLOAT3 newScale);

	// --------------------------------------------------------
	// Set the scale for this GameObject
	//
	// x - new x scale
	// y - new y scale
	// z - new z scale
	// --------------------------------------------------------
	void SetScale(float x, float y, float z);

	// --------------------------------------------------------
	// Get this object's collider
	// --------------------------------------------------------
	Collider* GetCollider();

	// --------------------------------------------------------
	// Add a collider to this object if it has none
	//
	// size - dimensions of bounding box
	// offset - offset of collider from position of game object
	// --------------------------------------------------------
	void AddCollider(DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset = DirectX::XMFLOAT3());

	// --------------------------------------------------------
	// Check if the collider is in debug mode (draw outline)
	// --------------------------------------------------------
	bool IsDebug();

	// --------------------------------------------------------
	// Set debug mode for this collider (draw outline)
	// --------------------------------------------------------
	void SetDebug(bool setting);
};

