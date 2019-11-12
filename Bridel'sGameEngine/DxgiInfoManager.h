#pragma once

#include "WindowsInclude.h"
#include <wrl.h>
#include <vector>
#include <dxgidebug.h>
#include <string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void set() noexcept;
	std::vector<std::string> getMessages() const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};