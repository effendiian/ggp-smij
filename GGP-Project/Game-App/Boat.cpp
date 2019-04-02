#include "Boat.h"

using namespace std;
using namespace DirectX;

Boat::Boat(Mesh * mesh, Material * material, DirectX::XMFLOAT3 position) : Entity(mesh, material)
{
	this->SetPosition(position);

	crashed = false;

	nodes = vector<GameObject*>();
	nodes.push_back(this);

	timer = 0;
	path = vector<XMFLOAT3>();
}

Boat::~Boat()
{
}

void Boat::Update()
{
	Input();

	Move();

	CheckCollisions();
}

void Boat::Input()
{
}

void Boat::Move()
{
	XMVECTOR forward = XMLoadFloat3(&this->GetForwardAxis());
	XMFLOAT3 displacement;
	XMStoreFloat3(&displacement, forward * speed);

	this->MoveRelative(displacement);
}

void Boat::CheckCollisions()
{
}
