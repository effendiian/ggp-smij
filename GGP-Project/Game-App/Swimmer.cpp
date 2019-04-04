#include "Swimmer.h"

using namespace DirectX;

//Snake logic from:
//https://github.com/rimij405/ggp-smij/blob/Unity-Prototype/Prototype/Boat-Snake-Prototype/Assets/Scripts/BoatFollower.cs

Swimmer::Swimmer(Mesh* mesh, Material* material)
	: Swimmer(mesh, material, nullptr) {
	this->StopFollowing();
}

Swimmer::Swimmer(Mesh* mesh, Material* material, GameObject* leader) 
	: Entity(mesh, material)
{
	//Create buffers
	bufferLength = (int)ceil(lagSeconds * MAX_FPS);
	positionBuffer = new XMFLOAT3[bufferLength];
	timeBuffer = new float[bufferLength];

	//Set default vals
	this->leader = leader;
	positionBuffer[0] = positionBuffer[1] = (leader != nullptr) ? leader->GetPosition() : DirectX::XMFLOAT3(0, 0, 0);
	timeBuffer[0] = timeBuffer[1] = timer = 0;

	oldestIndex = 0;
	newestIndex = 1;
}

Swimmer::~Swimmer()
{
	//Delete buffers
	delete[] positionBuffer;
	delete[] timeBuffer;
}

void Swimmer::Update(float deltaTime)
{
	// If currently following:
	if (this->IsFollower()) {

		// Update internal timer.
		timer += deltaTime;

		// Insert newest position into our cache.
		// If the cache is full, overwrite the latest sample.
		int newIndex = (newestIndex + 1) % bufferLength;
		if (newIndex != oldestIndex)
			newestIndex = newIndex;

		positionBuffer[newestIndex] = leader->GetPosition();
		timeBuffer[newestIndex] = timer;

		// Skip ahead in the buffer to the segment containing our target time.
		float targetTime = timer - lagSeconds;
		int nextIndex;
		while (timeBuffer[nextIndex = (oldestIndex + 1) % bufferLength] < targetTime)
			oldestIndex = nextIndex;

		// Interpolate between the two samples on either side of our target time.
		float span = timeBuffer[nextIndex] - timeBuffer[oldestIndex];
		float progress = 0;
		if (span > 0)
		{
			progress = (targetTime - timeBuffer[oldestIndex]) / span;
		}

		//Lerp to find the position
		XMFLOAT3 lerp;
		XMStoreFloat3(&lerp, XMVectorLerp(XMLoadFloat3(&(positionBuffer[oldestIndex])), XMLoadFloat3(&(positionBuffer[nextIndex])), progress));
		SetPosition(lerp);
	}
	
	// If not following:
	if(!this->IsFollower())
	{
		// Rotate when idle.
		XMFLOAT3 nextRotation = this->GetRotation();
		nextRotation.x += 5 * deltaTime;
		this->SetRotation(nextRotation);
	}
}

bool Swimmer::IsFollower() 
{
	return (this->leader != nullptr);
}

void Swimmer::StartFollowing(GameObject* newLeader) 
{
	this->leader = newLeader;
}

void Swimmer::StopFollowing()
{
	this->StartFollowing(nullptr);
}