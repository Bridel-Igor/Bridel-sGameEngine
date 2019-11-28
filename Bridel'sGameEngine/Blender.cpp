#include "Blender.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Blender::Blender(Graphics& gfx, bool blending)
		:
		blending(blending)
	{
		INFOMAN(gfx);

		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		auto& brt = blendDesc.RenderTarget[0];
		if (blending)
		{
			brt.BlendEnable = TRUE;
			brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			brt.BlendOp = D3D11_BLEND_OP_ADD;
			brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
			brt.DestBlendAlpha = D3D11_BLEND_ZERO;
			brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		GFX_THROW_INFO(getDevice(gfx)->CreateBlendState(&blendDesc, &pBlender));
	}

	void Bind::Blender::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->OMSetBlendState(pBlender.Get(), nullptr, 0xFFFFFFFFu);
	}

	std::shared_ptr<Blender> Blender::resolve(Graphics& gfx, bool blending)
	{
		return Codex::resolve<Blender>(gfx, blending);
	}

	std::string Blender::generateUID(bool blending)
	{
		using namespace std::string_literals;
		return typeid(Blender).name() + "#"s + (blending ? "b" : "n");
	}

	std::string Blender::getUID() const noexcept
	{
		return generateUID(blending);
	}
}