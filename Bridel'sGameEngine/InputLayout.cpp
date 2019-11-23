#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(getDevice(gfx)->CreateInputLayout(
			layout.data(), (UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}

	void InputLayout::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}