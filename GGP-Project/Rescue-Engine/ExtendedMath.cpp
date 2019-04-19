#include "ExtendedMath.h"
#include <algorithm>
//#include <cstdlib>

using namespace DirectX;
using namespace std;

//Clamp a value between a range
float ExtendedMath::Clamp(float value, float min, float max)
{
	return std::max(min, std::min(value, max));
}

//Returns whether the given value is in a min-max range.
bool ExtendedMath::InRange(float value, float min, float max)
{
	return value > min && value < max;
}

//Returns whether the given value is in a positive-negative single-value range.
bool ExtendedMath::InRange(float value, float range)
{
	return value > -range && value < range;
}

//Lerps until the result is negligably close to the target value.
float ExtendedMath::LerpThreshhold(float v1, float v2, float t, float thresh)
{
	if (abs(v1 - v2) > thresh)
		return v1 + t * (v2 - v1);
	else
		return v2;
	return 0;
}

//Calculate the distance between 2 Float3s
float ExtendedMath::DistanceFloat3(XMFLOAT3 v1, XMFLOAT3 v2)
{
	XMFLOAT3 dist;
	XMStoreFloat3(&dist, XMVector3Length(
		XMVectorSubtract(
			XMLoadFloat3(&v1), XMLoadFloat3(&v2))
	));
	return dist.x;
}

//Extract a quaternion from a rotation matrix
//https://forum.unity.com/threads/how-to-assign-matrix4x4-to-transform.121966/
XMFLOAT4 ExtendedMath::MatrixToQuaternion(XMFLOAT4X4 m)
{
	float tr = m._11 + m._22 + m._33;
	float w, x, y, z;
	if (tr > 0.0f) {
		float s = sqrt(1.0f + tr) * 2.0f;
		w = 0.25f * s;
		x = (m._32 - m._23) / s;
		y = (m._13 - m._31) / s;
		z = (m._21 - m._12) / s;
	}
	else if ((m._11 > m._22) && (m._11 > m._33)) {
		float s = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
		w = (m._32 - m._23) / s;
		x = 0.25f * s;
		y = (m._12 + m._21) / s;
		z = (m._13 + m._31) / s;
	}
	else if (m._22 > m._33) {
		float s = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
		w = (m._13 - m._31) / s;
		x = (m._12 + m._21) / s;
		y = 0.25f * s;
		z = (m._23 + m._32) / s;
	}
	else {
		float s = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
		w = (m._21 - m._12) / s;
		x = (m._13 + m._31) / s;
		y = (m._23 + m._32) / s;
		z = 0.25f * s;
	}

	XMFLOAT4 quat = XMFLOAT4(x, y, z, w);
	return quat;
}

//https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
XMFLOAT3 ExtendedMath::Vec3Slerp(XMFLOAT3 start, XMFLOAT3 end, float t)
{
	//Load start and end vectors
	XMVECTOR startVec = XMLoadFloat3(&start);
	XMVECTOR endVec = XMLoadFloat3(&end);

	// Dot product - the cosine of the angle between 2 vectors.
	XMFLOAT3 dotVec;
	XMStoreFloat3(&dotVec, XMVector3Dot(startVec, endVec));
	float dot = dotVec.x;

	// Clamp it to be in the range of Acos()
	// This may be unnecessary, but floating point
	// precision can be a fickle mistress.
	dot = Clamp(dot, -1.0f, 1.0f);

	// Acos(dot) returns the angle between start and end,
	// And multiplying that by percent returns the angle between
	// start and the final result.
	float theta = acos(dot) * t;
	XMVECTOR relativeVec = XMVector3Normalize(XMVectorSubtract(endVec, XMVectorScale(startVec, dot)));

	// Orthonormal basis
	// The final result.
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVectorAdd(
		XMVectorScale(startVec, cos(theta)),
		XMVectorScale(relativeVec, sin(theta))));
	return result;
}
