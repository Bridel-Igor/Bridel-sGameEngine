#pragma once
#include "Bindable.h"
#include <array>
#include <optional>

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
		void bind(Graphics& gfx) noexcept override;
		void setFactor(float factor) noxnd;
		float getFactor() const noxnd;
		static std::shared_ptr<Blender> resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
		static std::string generateUID(bool blending, std::optional<float> factor);
		std::string getUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}