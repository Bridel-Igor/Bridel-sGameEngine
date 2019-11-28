#pragma once

#include "Surface.h"
#include "MathInclude.h"
#include <string>
#include <DirectXMath.h>

class NormalMapTwerker
{
public:
	static void rotateXAxis180(const std::string& pathIn, const std::string& pathOut)
	{
		using namespace DirectX;
		const auto rotation = XMMatrixRotationX(PI);
		auto sIn = Surface::fromFile(pathIn);

		const auto nPixels = sIn.getWidth() * sIn.getHeight();
		const auto pBegin = sIn.getBufferPtr();
		const auto pEnd = sIn.getBufferPtrConst() + nPixels;
		for (auto pCurrent = pBegin; pCurrent < pEnd; pCurrent++)
		{
			auto n = colorToVerctor(*pCurrent);
			n = XMVector3Transform(n, rotation);
			*pCurrent = vectorToColor(n);
		}

		sIn.save(pathOut);
	}
	static void rotaXAxis180(const std::string& pathIn)
	{
		return rotateXAxis180(pathIn, pathIn);
	}
private:
	static DirectX::XMVECTOR colorToVerctor(Surface::Color c)
	{
		using namespace DirectX;
		auto n = XMVectorSet((float)c.getR(), (float)c.getG(), (float)c.getB(), 0.0f);
		const auto all255 = XMVectorReplicate(2.0f / 255.0f);
		n = XMVectorMultiply(n, all255);
		const auto all1 = XMVectorReplicate(1.0f);
		n = XMVectorSubtract(n, all1);
		return n;
	}
	static Surface::Color vectorToColor(DirectX::FXMVECTOR n)
	{
		using namespace DirectX;
		const auto all1 = XMVectorReplicate(1.0f);
		XMVECTOR nOut = XMVectorAdd(n, all1);
		const auto all255 = XMVectorReplicate(255.0f / 2.0f);
		nOut = XMVectorMultiply(nOut, all255);
		XMFLOAT3 floats;
		XMStoreFloat3(&floats, nOut);
		return{
			(unsigned char)round(floats.x),
			(unsigned char)round(floats.y),
			(unsigned char)round(floats.z),
		};
	}
};