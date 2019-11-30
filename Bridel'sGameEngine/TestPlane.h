#pragma once
#include "Drawable.h"

class TestPlane : public Drawable
{
public:
	TestPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 0.0f });
	void setPos(DirectX::XMFLOAT3 pos) noexcept;
	void setRotation(float roll, float pitch, float yaw) noexcept;
	DirectX::XMMATRIX getTransformXM() const noexcept override;
	void spawnControlWindow(Graphics& gfx, const std::string& name) noexcept;
private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color;
	} pmc;
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};