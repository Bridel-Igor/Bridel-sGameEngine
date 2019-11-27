#include "TransformCbuf.h"

namespace Bind
{
	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
		:
		parent(parent)
	{
		if (!pVcbuf)
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}

	void TransformCbuf::bind(Graphics& gfx) noexcept
	{
		updateBindImpl(gfx, getTransforms(gfx));
	}

	void TransformCbuf::updateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		pVcbuf->update(gfx, tf);
		pVcbuf->bind(gfx);
	}

	TransformCbuf::Transforms TransformCbuf::getTransforms(Graphics& gfx) noexcept
	{
		const auto modelView = parent.getTransformXM() * gfx.getCamera();
		return {	DirectX::XMMatrixTranspose(modelView),
					DirectX::XMMatrixTranspose(
						modelView *
						gfx.getProjection()
					)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
}