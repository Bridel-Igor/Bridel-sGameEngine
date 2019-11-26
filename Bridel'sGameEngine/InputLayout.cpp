#include "InputLayout.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx,
		Dvtx::VertexLayout layout_in,
		ID3DBlob* pVertexShaderBytecode)
		:
		layout(std::move(layout_in))
	{
		INFOMAN(gfx);

		const auto d3dLayout = layout.getD3DLayout();

		GFX_THROW_INFO(getDevice(gfx)->CreateInputLayout(
			d3dLayout.data(), (UINT)d3dLayout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}

	void InputLayout::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}

	std::shared_ptr<InputLayout> InputLayout::resolve(Graphics& gfx, const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		return Codex::resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
	}

	std::string InputLayout::generateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.getCode();
	}

	std::string InputLayout::getUID() const noexcept
	{
		return generateUID(layout);
	}
}