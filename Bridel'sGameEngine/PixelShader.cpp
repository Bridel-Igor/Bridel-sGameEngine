#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::string& path)
	{
		INFOMAN(gfx);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{path.begin(), path.end()}.c_str(), &pBlob));
		GFX_THROW_INFO(getDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void PixelShader::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	std::shared_ptr<PixelShader> PixelShader::resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::resolve<PixelShader>(gfx, path);
	}

	std::string PixelShader::generateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
	}

	std::string PixelShader::getUID() const noexcept
	{
		return generateUID(path);
	}
}