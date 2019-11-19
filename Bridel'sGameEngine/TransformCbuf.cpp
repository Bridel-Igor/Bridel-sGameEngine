#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
}

void TransformCbuf::bind(Graphics& gfx) noexcept
{
	const auto model = parent.getTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.getCamera() *
			gfx.getProjection()
		)
	};
			
	pVcbuf->update(gfx, tf);
	pVcbuf->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;