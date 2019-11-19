#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void update(float dt) noexcept override;
	void setPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX getTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f, 1.0f, 1.0f };
};