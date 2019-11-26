#pragma once
#include "Bindable.h"

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Sampler> resolve(Graphics& gfx);
		static std::string generateUID();
		std::string getUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}