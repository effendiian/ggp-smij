#pragma once
#include <DirectXMath.h>

class Collider
{
private:
	DirectX::XMFLOAT3 position; //center
	DirectX::XMFLOAT3 offset; //(0,0,0) if not given
	DirectX::XMFLOAT3 size; //xyz = width, height, length

public:
	Collider();
	Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size);
	Collider(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 offset); //optional center offset
	~Collider();

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetSize() const;
	DirectX::XMFLOAT3 GetHalfSize() const;
	void SetPosition(DirectX::XMFLOAT3 newPosition);

	bool Collides(Collider other); //AABB for now
};

