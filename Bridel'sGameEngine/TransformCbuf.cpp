#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
		pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
}

void TransformCbuf::bind(Graphics& gfx) noexcept
{
	pVcbuf->update(gfx,
		DirectX::XMMatrixTranspose(
			parent.getTransformXM() * gfx.getProjection() 
		)
	);
	pVcbuf->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pVcbuf;