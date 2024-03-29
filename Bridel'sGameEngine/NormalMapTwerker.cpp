#include "NormalMapTwerker.h"
#include <filesystem>
#include "Mesh.h"
#include "MathInclude.h"

template<typename F>
inline void NormalMapTwerker::transformFile(const std::string& pathIn, const std::string& pathOut, F&& func)
{
	auto surf = Surface::fromFile(pathIn);

	const auto nPixels = surf.getWidth() * surf.getHeight();
	const auto pBegin = surf.getBufferPtr();
	const auto pEnd = surf.getBufferPtrConst() + nPixels;
	for (auto pCurrent = pBegin; pCurrent < pEnd; pCurrent++)
	{
		const auto n = colorToVector(*pCurrent);
		*pCurrent = vectorToColor(func(n));
	}

	surf.save(pathOut);
}

void NormalMapTwerker::flipYAllNormalMapsInObj(const std::string& objPath)
{
	const auto rootPath = std::filesystem::path{ objPath }.parent_path().string() + "\\";

	// load scene from .obj file to get our list of normal maps in the materials
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(objPath.c_str(), 0u);
	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	// function for processing each normal in texture
	using namespace DirectX;
	const auto flipY = XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f);
	const auto ProcessNormal = [flipY](FXMVECTOR n) -> XMVECTOR
	{
		return XMVectorMultiply(n, flipY);
	};

	// loop through materials and process any normal maps
	for (auto i = 0u; i < pScene->mNumMaterials; i++)
	{
		const auto& mat = *pScene->mMaterials[i];
		aiString texFileName;
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			const auto path = rootPath + texFileName.C_Str();
			transformFile(path, path, ProcessNormal);
		}
	}
}

DirectX::XMVECTOR NormalMapTwerker::colorToVector(Surface::Color c) noexcept
{
	using namespace DirectX;
	auto n = XMVectorSet((float)c.getR(), (float)c.getG(), (float)c.getB(), 0.0f);
	const auto all255 = XMVectorReplicate(2.0f / 255.0f);
	n = XMVectorMultiply(n, all255);
	const auto all1 = XMVectorReplicate(1.0f);
	n = XMVectorSubtract(n, all1);
	return n;
}

Surface::Color NormalMapTwerker::vectorToColor(DirectX::FXMVECTOR n) noexcept
{
	using namespace DirectX;
	const auto all1 = XMVectorReplicate(1.0f);
	XMVECTOR nOut = XMVectorAdd(n, all1);
	const auto all255 = XMVectorReplicate(255.0f / 2.0f);
	nOut = XMVectorMultiply(nOut, all255);
	XMFLOAT3 floats;
	XMStoreFloat3(&floats, nOut);
	return {
		(unsigned char)round(floats.x),
		(unsigned char)round(floats.y),
		(unsigned char)round(floats.z),
	};
}