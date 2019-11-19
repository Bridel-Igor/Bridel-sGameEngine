#include "App.h"
#include "Melon.h"
#include "Pyramid.h"
#include "Box.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "MathInclude.h"
#include "Surface.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(720, 480, "Form")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(gfx, rng, adist, ddist, odist, rdist);
			case 1:
				return std::make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist);
			case 2:
				return std::make_unique<Melon>(gfx, rng, adist, ddist, odist, rdist, longdist, latdist);
			case 3: 
				return std::make_unique<Sheet>(gfx, rng, adist, ddist, odist, rdist);
			case 4:
				return std::make_unique<SkinnedBox>(gfx, rng, adist, ddist, odist, rdist);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, PI * 1.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		std::uniform_int_distribution<int> latdist{ 5, 20 };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_int_distribution<int> typedist{ 0, 4 };
	};
	
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{wnd.gfx()});

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

	const auto dt = timer.mark();
	wnd.gfx().clearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& d : drawables)
	{
		d->update(wnd.kbd.keyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->draw(wnd.gfx());
	}
	wnd.gfx().endFrame();
}