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

	// Evaluates a rotation needed to be applied to an object positioned at sourcePoint to face destPoint
	// sourcePoint - Coordinates of source point
	// destPoint - Coordinates of destination point
	static DirectX::XMFLOAT4 QuaternionLookAt(DirectX::XMFLOAT3 sourcePoint, DirectX::XMFLOAT3 destPoint,
		DirectX::XMFLOAT3 front, DirectX::XMFLOAT3 up)
	{
		DirectX::XMVECTOR toVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&destPoint), DirectX::XMLoadFloat3(&sourcePoint)));

		//compute rotation axis
		DirectX::XMVECTOR rotAxis = DirectX::XMVector3Normalize(
			DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&front), toVector));
		DirectX::XMFLOAT3 dot1;
		DirectX::XMStoreFloat3(&dot1, DirectX::XMVector3Dot(rotAxis, rotAxis));
		if (dot1.x == 0)
			rotAxis = XMLoadFloat3(&up);

		//find the angle around rotation axis
		DirectX::XMFLOAT3 dot2;
		DirectX::XMStoreFloat3(&dot2, 
			DirectX::XMVector3Dot(DirectX::XMVectorSet(1, 0, 0, 0), toVector));
		float dot = dot2.x;
		float ang = std::acosf(dot);

		//convert axis angle to quaternion
		DirectX::XMFLOAT3 axis;
		DirectX::XMStoreFloat3(&axis, rotAxis);
		return AngleAxis(axis, ang);
	}

	//Angle-Axis to Quaternion
	static DirectX::XMFLOAT4 AngleAxis(DirectX::XMFLOAT3 axis, float angle) {
		float s = std::sinf(angle / 2);
		DirectX::XMFLOAT3 u;
		DirectX::XMStoreFloat3(&u, DirectX::XMVector3Normalize(XMLoadFloat3(&axis)));
		return DirectX::XMFLOAT4(std::cosf(angle / 2), u.x * s, u.y * s, u.z * s);
	}
};
