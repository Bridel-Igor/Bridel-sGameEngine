#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(720, 480, "Form")
{}

int App::go()
{
	MSG msg;
	BOOL gResult;
	while (true)
	{
		// process all messages pending, but to not block
		if (const auto ecode = Window::processMessages())
		{
			// if return optional has value, means we're quitting
			return *ecode;
		}
		Sleep(1);
		doFrame();
	}

	// check if getMessage call itself borked
	if (gResult == -1)
		throw WND_LAST_EXCEPT();

	return msg.wParam;
}

void App::doFrame()
{
	if (wnd.kbd.keyIsPressed(VK_ESCAPE))
		PostQuitMessage(0);

	if (!wnd.kbd.charIsEmpty())
	{
		char c = wnd.kbd.readChar().value();
		if (c != VK_BACK)
			word = word + c;
		else
			if (word.length() > 0)
				word.erase(word.length() - 1);
	}

	while (!wnd.mouse.isEmpty())
	{
		const auto e = wnd.mouse.read();
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
	std::ostringstream oss;
	const float t = timer.peek();
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s ";
	if (mouseInClient)
		oss << "Mouse enter client. ";
	else
		oss << "Mouse leave client. ";
	oss << "Mouse Position: (" << x << "," << y << "). ";
	oss << "Wheel pos: " << wheelRoll << " ";
	oss << "String: " << word;
	wnd.setTitle(oss.str());

	const float c = sin(timer.peek()) / 2.0f + 0.5f;
	wnd.gfx().clearBuffer(c, c, 1.0f);
	wnd.gfx().drawTestTriangle();
	wnd.gfx().endFrame();
}