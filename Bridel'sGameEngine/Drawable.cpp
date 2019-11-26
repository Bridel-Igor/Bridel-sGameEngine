#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>

using namespace Bind;

void Drawable::draw(Graphics& gfx) const noxnd
{
	for (auto& b : binds)
		b->bind(gfx);
	gfx.drawIndexed(pIndexBuffer->getCount());
}

void Drawable::addBind(std::shared_ptr<Bindable> bind) noxnd
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back(std::move(bind));
}