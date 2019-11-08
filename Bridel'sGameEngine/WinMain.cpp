#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		Window window(640, 480, "Form");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult == -1)
			return -1;
		return msg.wParam;
	}
	catch (const ExceptionFinder & e)
	{
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{
		MessageBox(nullptr, e.what(), "Standart Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}