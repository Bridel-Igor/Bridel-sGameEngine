#pragma once

#include "WindowsInclude.h"

class Window
{
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
};