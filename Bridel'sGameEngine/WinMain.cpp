#include "Window.h"
#include <sstream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		Window window(640, 480, "Form");
		window.kbd.disableAutorepeat();
		
		MSG msg;
		BOOL gResult;

		
		int wheelRoll = 0, x=0, y=0;//
		bool mouseInClient = false;//
		std::string word;//
		window.kbd.disableAutorepeat();

		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (window.kbd.keyIsPressed(VK_ESCAPE))
				PostQuitMessage(0);
			
			if (!window.kbd.charIsEmpty())
			{
				char c = window.kbd.readChar().value();
				if (c != VK_BACK)
					word = word + c;
				else 
					if (word.length() > 0)
						word.erase(word.length() - 1);
			}

			std::ostringstream oss;
			while (!window.mouse.isEmpty())
			{
				const auto e = window.mouse.read();
				switch (e.getType())
				{
				case Mouse::Event::Type::Leave:
					mouseInClient = false;
					break;
				case Mouse::Event::Type::Enter:
					mouseInClient = true;
					break;
				case Mouse::Event::Type::WheelUp: 
					wheelRoll++;
					break; 
				case Mouse::Event::Type::WheelDown: 
					wheelRoll--;
					break; 
				case Mouse::Event::Type::Move:
					x = e.getPosX();
					y = e.getPosY();
					break;
				}
			}
			if (mouseInClient)
				oss << "Mouse enter client. ";
			else
				oss << "Mouse leave client. ";
			oss << "Mouse Position: (" << x << "," << y << "). ";
			oss << "Wheel pos: " << wheelRoll << " ";
			oss << "String: " << word;
			window.setTitle(oss.str());
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