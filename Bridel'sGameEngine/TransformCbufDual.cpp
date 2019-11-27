#include "TransformCbufDual.h"

namespace Bind
{
	TransformCbufDual::TransformCbufDual(Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP)
		:
		TransformCbuf(gfx, parent, slotV)
	{
		if (!pPcbuf)
			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
	}

	void TransformCbufDual::bind(Graphics& gfx) noexcept
	{
		const auto tf = getTransforms(gfx);
		TransformCbuf::updateBindImpl(gfx, tf);
		updateBindImpl(gfx, tf);
	}

	void TransformCbufDual::updateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		pPcbuf->update(gfx, tf);
		pPcbuf->bind(gfx);
	}

	std::unique_ptr<PixelConstantBuffer<TransformCbuf::Transforms>> TransformCbufDual::pPcbuf;
}