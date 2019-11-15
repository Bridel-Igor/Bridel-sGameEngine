#include "App.h"
#include "Box.h"
#include <sstream>
#include <iomanip>
#include <memory>

App::App()
	:
	wnd(720, 480, "Form")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 480.0f / 720.0f, 0.5f, 40.0f));
}

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

App::~App()
{
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
	if (mouseInClient)
		oss << "Mouse enter client. ";
	else
		oss << "Mouse leave client. ";
	oss << "Mouse Position: (" << x << "," << y << "). ";
	oss << "Wheel pos: " << wheelRoll << " ";
	oss << "String: " << word;
	wnd.setTitle(oss.str());

	auto dt = timer.mark();
	wnd.gfx().clearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->update(dt);
		b->draw(wnd.gfx());
	}
	wnd.gfx().endFrame();
}