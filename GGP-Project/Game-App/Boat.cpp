#include "Boat.h"
#include "ResourceManager.h"

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	crashed = false;
	inputManager = InputManager::GetInstance();
}

Boat::~Boat()
{ 
	//TODO: Offload swimmer management to entity manager
	for (int i = 0; i < swimmers.size(); i++)
	{
		if (swimmers[i]) delete swimmers[i];
	}
}

// Calls Input, Move, and CheckCollisions every frame
void Boat::Update(float deltaTime)
{
	Input(deltaTime);

	Move(deltaTime);

	//TODO: Offload swimmer management to entity manager
	for (int i = 0; i < swimmers.size(); i++)
	{
		swimmers[i]->Update(deltaTime);
	}

	CheckCollisions();
}

// Interprets key input
void Boat::Input(float deltaTime)
{
	//Left
	if (inputManager->GetKey(VK_LEFT))
	{
		this->Rotate(0, turnSpeed * deltaTime * -1, 0);
	}
	//Right
	else if (inputManager->GetKey(VK_RIGHT))
	{
		this->Rotate(0, turnSpeed * deltaTime * 1, 0);
	}

//If we are in debug, allow us to click to spawn swimmers on our tail
#if defined(DEBUG) || defined(_DEBUG)
	if (inputManager->GetMouseButtonDown(MouseButtons::L))
	{
		CreateSwimmer();
	}
#endif
}

// Moves the boat forward
void Boat::Move(float deltaTime)
{
	this->MoveRelative(XMFLOAT3(speed * deltaTime, 0, 0));
}

// Checks for collisions and calls corresponding collide methods
void Boat::CheckCollisions()
{
}

// Create a swimmer at the end of the trail
void Boat::CreateSwimmer()
{
	//TODO: Add swimmer to entity manager

	//Get leader for the new swimmer
	GameObject* leader;
	if (swimmers.size() < 1)
		leader = this;
	else
		leader = swimmers[swimmers.size() - 1];

	//Create swimmer and add it to list
	Swimmer* swimmer = new Swimmer(ResourceManager::GetInstance()->GetMesh(swimmerMesh),
		ResourceManager::GetInstance()->GetMaterial(swimmerMat), leader);
	swimmer->SetPosition(leader->GetPosition());
	swimmer->SetRotation(leader->GetRotation());
	swimmers.push_back(swimmer);
}
