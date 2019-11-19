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
#include "ImGui/imgui.h"

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App()
	:
	wnd(720, 480, "Bridel'sGameEngine")
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

	wnd.gfx().setProjection(dx::XMMatrixPerspectiveLH(1.0f, 480.0f / 720.0f, 0.5f, 40.0f));
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
	const auto dt = timer.mark() * speed_factor;
	wnd.gfx().beginFrame(0.07f, 0.0f, 0.12f);
	wnd.gfx().setCamera(cam.getMatrix());

	for (auto& d : drawables)
	{
		d->update(wnd.kbd.keyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->draw(wnd.gfx());
	}

	static char buffer[1024];
	// imgui window to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.keyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING");
	}
	ImGui::End();
	// imgui window to control camera
	cam.spawnControlWindow();

	// present
	wnd.gfx().endFrame();
}