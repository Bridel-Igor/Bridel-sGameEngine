#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	vcbuf(gfx),
	parent(parent)
{}

void TransformCbuf::bind(Graphics& gfx) noexcept
{
	vcbuf.update(gfx,
		DirectX::XMMatrixTranspose(
			parent.getTransformXM() * gfx.getProjection() 
		)
	);
	vcbuf.bind(gfx);
}
