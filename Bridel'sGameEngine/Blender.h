#pragma once
#include "Bindable.h"
#include <array>

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx, bool blending);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Blender> resolve(Graphics& gfx, bool blending);
		static std::string generateUID(bool blending);
		std::string getUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		bool blending;
	};
}