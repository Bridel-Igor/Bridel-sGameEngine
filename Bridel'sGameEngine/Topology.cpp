#include "Topology.h"
#include "BindableCodex.h"

namespace Bind
{
	Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{}

	void Topology::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->IASetPrimitiveTopology(type);
	}

	std::shared_ptr<Topology> Topology::resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	{
		return Codex::resolve<Topology>(gfx, type);
	}

	std::string Topology::generateUID(D3D11_PRIMITIVE_TOPOLOGY type)
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string(type);
	}

	std::string Topology::getUID() const noexcept
	{
		return generateUID(type);
	}
}