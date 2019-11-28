#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphics& gfx, bool twoSided)
		:
		twoSided(twoSided)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GFX_THROW_INFO(getDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	void Rasterizer::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::resolve(Graphics& gfx, bool twoSided)
	{
		return Codex::resolve<Rasterizer>(gfx, twoSided);
	}

	std::string Rasterizer::generateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}

	std::string Rasterizer::getUID() const noexcept
	{
		return generateUID(twoSided);
	}
}