#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* getContext(Graphics& gfx) noexcept;
	static ID3D11Device* getDevice(Graphics& gfx) noexcept;
	static DxgiInfoManager& getInfoManager(Graphics& gfx) noexcept(!IS_DEBUG);
};