#pragma once
#include <DirectXMath.h>

class ExtendedMath
{

public:

	//Clamp a value between a range
	static float Clamp(float value, float min, float max);

	//Returns whether the given value is in a min-max range.
	static bool InRange(float value, float min, float max);

	//Returns whether the given value is in a positive-negative single-value range.
	static bool InRange(float value, float range);

	//Lerps until the result is negligably close to the target value.
	static float LerpThreshhold(float v1, float v2, float t, float thresh = .001);
	
	//Calculate the distance between 2 Float3s
	static float DistanceFloat3(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	//Extract a quaternion from a rotation matrix
	//https://forum.unity.com/threads/how-to-assign-matrix4x4-to-transform.121966/
	static DirectX::XMFLOAT4 MatrixToQuaternion(DirectX::XMFLOAT4X4 m);

	//Slerp a Vector3
	//https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
	static DirectX::XMFLOAT3 Vec3Slerp(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float t);
};
