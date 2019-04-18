#pragma once
#include <cstdlib>
#include <DirectXMath.h>
#include "iostream"
#include <ctime>

using namespace DirectX;

class ExtendedMath
{

public:

	//Returns whether the given value is in a min-max range.
	static bool InRange(float value, float min, float max)
	{
		return value > min && value < max;
	}

	//Returns whether the given value is in a positive-negative single-value range.
	static bool InRange(float value, float range)
	{
		return value > -range && value < range;
	}

	//Lerps until the result is negligably close to the target value.
	static float LerpThreshhold(float v1, float v2, float t, float thresh = .001)
	{
		if (abs(v1 - v2) > thresh)
			return v1 + t * (v2 - v1);
		else
			return v2;
		return 0;
	}
	
	//Calculate the distance between 2 Float3s
	static float DistanceFloat3(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
	{
		DirectX::XMFLOAT3 dist;
		DirectX::XMStoreFloat3(&dist, DirectX::XMVector3Length(
			DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2))
		));
		return dist.x;
	}

	//Returns a random decimal from min -> max.
	static float Random(int min = 0, int max = 1)
	{
		srand(unsigned(time(NULL)));
		return rand() / (max + 1) + min + (rand() % max - min);
	}

	//Returns a random vector on a flat plane.
	static XMFLOAT3 RandomVector2D()
	{
		float a = Random() * 2.0f * 3.1415927f;
		return XMFLOAT3(cos(a), sin(a), 0.0f);
	}

	//Returns a random unit circle vector.
	static XMFLOAT3 RandomVector3D()
	{
		float z = (2 * Random()) - 1;

		XMFLOAT3 p   = RandomVector2D();
		XMVECTOR xmp = XMLoadFloat3(&p);

		xmp *= sqrt(1 - z * z);
		XMStoreFloat3(&p, xmp);

		p   = XMFLOAT3(p.x, p.y, z);
		xmp = XMLoadFloat3(&p);
		xmp = XMVector3Normalize(xmp);

		XMStoreFloat3(&p, xmp);
		return p;
	}

	//Returns a random vector in the up-direction.
	static XMFLOAT3 RandomSplashVector()
	{
		XMFLOAT3 v = RandomVector3D();
		v.y = abs(v.y);
		return v;
	}
};
