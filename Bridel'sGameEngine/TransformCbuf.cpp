#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (!pVcbuf)
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
}

void TransformCbuf::bind(Graphics& gfx) noexcept
{
	const auto modelView = parent.getTransformXM() * gfx.getCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.getProjection()
		)
	};
			
	pVcbuf->update(gfx, tf);
	pVcbuf->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;