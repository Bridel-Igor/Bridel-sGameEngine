#include "VertexShader.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
		GFX_THROW_INFO(getDevice(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		));
	}

	void VertexShader::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::getBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
}