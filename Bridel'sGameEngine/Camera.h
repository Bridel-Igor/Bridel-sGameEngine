#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera() noexcept;
	DirectX::XMMATRIX getMatrix() const noexcept;
	void spawnControlWindow() noexcept;
	void reset() noexcept;
	void rotate(float dx, float dy) noexcept;
	void translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 getPos() const noexcept;
private:
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};