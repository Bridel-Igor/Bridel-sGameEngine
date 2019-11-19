#pragma once
#include "WindowsInclude.h"
#include "ExceptionFinder.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include <random>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DxgiInfoManager.h"

class Graphics
{
	friend class Bindable;
public:
	class Exception : public ExceptionFinder
	{
		using ExceptionFinder::ExceptionFinder;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		std::string getErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* getType() const noexcept override;
	private:
		std::string reason;
	};
	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void endFrame();
	void beginFrame(float red, float green, float blue) noexcept;
	void drawIndexed(UINT count) noexcept(!IS_DEBUG);
	void setProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX getProjection() const noexcept;
	void setCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX getCamera() const noexcept;
	void enableImGui() noexcept;
	void disableImGui() noexcept;
	bool isImGuiEnabled() const noexcept;
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
	bool imGuiEnabled = true;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	// ComPtr is used for garanteed Release() of Com objects even in case of exception
	// if you gain adress of com object through & it will release object before giving adress
	// if you need adress you should use pObj.GetAddressOf()
	// if you need object itself you should use pObj.Get()
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};