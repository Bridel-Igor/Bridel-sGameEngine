#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX getTransformXM() const noexcept = 0;
	void draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	virtual void update(float dt) noexcept = 0;
	void addBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void addIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	virtual ~Drawable() = default;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};