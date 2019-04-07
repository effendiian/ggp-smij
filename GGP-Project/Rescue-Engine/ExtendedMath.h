#pragma once
#include <cstdlib>
#include <DirectXMath.h>
#include "iostream"

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
};
