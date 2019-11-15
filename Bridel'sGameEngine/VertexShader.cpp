#include "VertexShader.h"
#include "GraphicsThrowMacros.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCodeBlob));
	GFX_THROW_INFO(getDevice(gfx)->CreateVertexShader(
		pByteCodeBlob->GetBufferPointer(), 
		pByteCodeBlob->GetBufferSize(), 
		nullptr, 
		&pVertexShader
	));
}

void VertexShader::bind(Graphics& gfx) noexcept
{
	getContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::getByteCode() const noexcept
{
	return pByteCodeBlob.Get();
}
