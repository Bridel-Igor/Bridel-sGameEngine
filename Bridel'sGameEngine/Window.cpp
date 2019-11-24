#include "Window.h"
#include <sstream>
#include "ImGui/imgui_impl_win32.h"

// Window Class
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass::getName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept
{
	return wndClass.hInst;
}


// Window
Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		throw WND_LAST_EXCEPT();
	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::getInstance(), this
	);
	// check for error
	if (hWnd == nullptr)
		throw WND_LAST_EXCEPT();
	// newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	// init ImGui Win32 implementation
	ImGui_ImplWin32_Init(hWnd);
	// create graphics object
	pGfx = std::make_unique<Graphics>(hWnd, width, height);
	// register mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		throw WND_LAST_EXCEPT();
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}

void Window::setTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
		throw WND_LAST_EXCEPT();
}

void Window::enableCursor() noexcept
{
	cursorEnabled = true;
	showCursor();
	enableImGuiMouse();
	freeCursor();
}

void Window::disableCursor() noexcept
{
	cursorEnabled = false;
	hideCursor();
	disableImGuiMouse();
	confineCursor();
}

bool Window::isCursorEnabled() const noexcept
{
	return cursorEnabled;
}

std::optional<int> Window::processMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block processing)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wParam)
			return msg.wParam;
		}

		// TranslateMessage will post auhilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

Graphics& Window::gfx()
{
	if (!pGfx)
		throw WND_NOGFX_EXCEPT();
	return *pGfx;
}

void Window::confineCursor() noexcept
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::freeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::hideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::showCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::enableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::disableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in form CreateWindow() to store window class pointer
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//forwsrd message to window class handler
	return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	const auto& imio = ImGui::GetIO();

	switch (msg)
	{
	// we don't want the DefProc to handle this message because
	// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	// clear keystate when window loses focus to prevent input getting from zombie keys
	case WM_KILLFOCUS:
		kbd.clearState();
		break;
	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				confineCursor();
				hideCursor();
			}
			else
			{
				freeCursor();
				showCursor();
			}
		}
		break;
	// keyboard messages
	case WM_KEYDOWN:
	// syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYDOWN:
		// stifle this keyboard message if ImGui wants to capture
		if (imio.WantCaptureKeyboard)
			break;
		if (!(lParam & 0x40000000) || kbd.autorepeatIsEnabled())
			kbd.onKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureKeyboard)
			break;
		kbd.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureKeyboard)
			break;
		kbd.onChar(static_cast<unsigned char>(wParam));
		break;
	// end of keyboard messages

	// mouse messages
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!cursorEnabled)
		{
			if (!mouse.isMouseInWindow())
			{
				SetCapture(hWnd);
				mouse.onMouseEnter();
				hideCursor();
			}
			break;
		}
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		// in client region -> log move, and log enter + capture mouse
		if (pt.x >= 0 && pt.x < width && width && pt.y >= 0 && pt.y < height)
		{
			mouse.onMouseMove(pt.x, pt.y);
			if (!mouse.isMouseInWindow())
			{
				SetCapture(hWnd);
				mouse.onMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
				mouse.onMouseMove(pt.x, pt.y);
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			confineCursor();
			hideCursor();
		}
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.onMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// stifle this keyboard message if imgui wants to capture
		if (imio.WantCaptureMouse)
			break;
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.onWheelDelta(pt.x, pt.y, delta);
		break;
	}
	// end of mouse messages

	// raw mouse messages
	case WM_INPUT:
	{
		if (!mouse.isRawEnabled())
			break;
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}
		rawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			// bail msg processing if error
			break;
		}
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.onRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	// end of raw messages
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Window Exception
std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make out pointer to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
		return "Udentified error code";
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception (line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] 0x" << std::hex << getErrorCode()
		<< std::dec << " (" << (unsigned long)getErrorCode() << ")" << std::endl
		<< "[Description] " << getErrorDescription() << std::endl
		<< getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::getType() const noexcept
{
	return "Window Exception";
}

HRESULT Window::HrException::getErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::getErrorDescription() const noexcept
{
	return Exception::translateErrorCode(hr);
}

const char* Window::NoGfxException::getType() const noexcept
{
	return "Window Exception [No Graphics]";
}