#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include <typeinfo>

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		:
		path(path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{path.begin(), path.end()}.c_str(), &pBytecodeBlob));
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

	std::shared_ptr<VertexShader> VertexShader::resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::resolve<VertexShader>(gfx, path);
	}

	std::string VertexShader::generateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}

	std::string VertexShader::getUID() const noexcept
	{
		return generateUID(path);
	}
}