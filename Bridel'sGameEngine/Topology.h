#pragma once
#include "Bindable.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Topology> resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string generateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string getUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}