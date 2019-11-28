#pragma once

#include "Surface.h"
#include <string>
#include <DirectXMath.h>


class NormalMapTwerker
{
public:
	static void flipYAllNormalMapsInObj(const std::string& objPath);
private:
	template<typename F>
	static void transformFile(const std::string& pathIn, const std::string& pathOut, F&& func);
	static DirectX::XMVECTOR colorToVector(Surface::Color c) noexcept;
	static Surface::Color vectorToColor(DirectX::FXMVECTOR n) noexcept;
};