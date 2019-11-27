#pragma once
#include "TransformCbuf.h"

namespace Bind
{
	class TransformCbufDual : public TransformCbuf
	{
	public:
		TransformCbufDual(Graphics& gfx, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u);
		void bind(Graphics& gfx) noexcept override;
	protected:
		void updateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
	};
}