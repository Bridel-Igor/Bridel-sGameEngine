#include "App.h"
#include "AssTest.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "MathInclude.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "ImGui/imgui.h"

#pragma comment (lib, "assimp-vc141-mtd.lib")

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App()
	:
	wnd(1280, 720, "Bridel'sGameEngine"),
	light(wnd.gfx())
{
	wnd.gfx().setProjection(dx::XMMatrixPerspectiveLH(1.0f, 720.0f / 1280.0f, 0.5f, 100.0f));
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
	light.bind(wnd.gfx(), cam.getMatrix());

	// render geometry
	nano.draw(wnd.gfx());
	light.draw(wnd.gfx());

	// imgui windows
	cam.spawnControlWindow();
	light.spawnControlWindow();
	showImguiDemoWindow();
	nano.showWindow();
	//present
	wnd.gfx().endFrame();
}

void App::showImguiDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
}
