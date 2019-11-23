#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>

using namespace Bind;

void Drawable::draw(Graphics& gfx) const noxnd
{
	for (auto& b : binds)
		b->bind(gfx);
	for (auto& b : getStaticBinds())
		b->bind(gfx);
	gfx.drawIndexed(pIndexBuffer->getCount());
}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noxnd
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::addIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noxnd
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
