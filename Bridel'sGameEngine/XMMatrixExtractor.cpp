#include "XMMatrixExtractor.h"

DirectX::XMFLOAT3 extractEulerAngles(const DirectX::XMFLOAT4X4& matrix)
{
	DirectX::XMFLOAT3 euler;

	euler.x = asinf(-matrix._32);					// pitch
	if (cosf(euler.x) > 0.0001)
	{
		euler.y = atan2f(matrix._31, matrix._33);	// yaw
		euler.z = atan2f(matrix._12, matrix._22);	// roll
	}
	else
	{
		euler.y = 0.0f;								// yaw
		euler.z = atan2f(-matrix._21, matrix._11);	// roll
	}

	return euler;
}

DirectX::XMFLOAT3 extractTranslation(const DirectX::XMFLOAT4X4& matrix)
{
	return {matrix._41, matrix._42, matrix._43};
}
