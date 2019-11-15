#include "Bindable.h"

ID3D11DeviceContext* Bindable::getContext(Graphics& gfx) noexcept
{
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::getDevice(Graphics& gfx) noexcept
{
	return gfx.pDevice.Get();
}

DxgiInfoManager& Bindable::getInfoManager(Graphics& gfx) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Tried to access gfx.infoManager in Release config");
#endif
}