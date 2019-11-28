#pragma once
#include "Bindable.h"
#include <array>

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(Graphics& gfx, bool twoSided);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Rasterizer> resolve(Graphics& gfx, bool twoSided);
		static std::string generateUID(bool twoSided);
		std::string getUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		bool twoSided;
	};
}