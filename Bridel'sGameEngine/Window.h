#pragma once

#include "WindowsInclude.h"
#include "ExceptionFinder.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

class Window
{
public:
	class WindowException : public ExceptionFinder
	{
	public:
		WindowException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept;
		static std::string translateErrorCode(HRESULT hr) noexcept;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Bridel's Game Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
	
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void setTitle(const std::string&);
	static std::optional<int> processMessages();

	Keyboard kbd;
	Mouse mouse;
};

// error exception helper macro
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError())