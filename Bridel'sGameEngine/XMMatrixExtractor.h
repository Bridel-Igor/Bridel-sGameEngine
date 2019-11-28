#pragma once
#include <DirectXMath.h>

DirectX::XMFLOAT3 extractEulerAngles(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT3 extractTranslation(const DirectX::XMFLOAT4X4& matrix);