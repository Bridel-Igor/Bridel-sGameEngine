#include "App.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "MathInclude.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "ImGui/imgui.h"
#include "VertexBuffer.h"

#pragma comment (lib, "assimp-vc141-mtd.lib")

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App()
	:
	wnd(1280, 720, "Bridel'sGameEngine"),
	light(wnd.gfx()),
	plane(wnd.gfx(), 3.0f)
{
	plane.setPos({ 1.0f, 17.0f, -1.0f });
	wnd.gfx().setProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
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
	nano2.draw(wnd.gfx());
	light.draw(wnd.gfx());
	plane.draw(wnd.gfx());

	while (const auto e = wnd.kbd.readKey())
	{
		if (!e->isPress())
			continue;
		switch (e->getCode())
		{
		case VK_ESCAPE:
			if (wnd.isCursorEnabled())
			{
				wnd.disableCursor();
				wnd.mouse.enableRaw();
			}
			else
			{
				wnd.enableCursor();
				wnd.mouse.disableRaw();
			}
			break;
		case VK_F1:
			if (wnd.isCursorEnabled())
				showDemoWindow = !showDemoWindow;
			break;
		}
	}

	if (!wnd.isCursorEnabled())
	{
		if (wnd.kbd.keyIsPressed('W'))
			cam.translate({ 0.0f, 0.0f, dt });
		if (wnd.kbd.keyIsPressed('A'))
			cam.translate({ -dt, 0.0f, 0.0f });
		if (wnd.kbd.keyIsPressed('S'))
			cam.translate({ 0.0f, 0.0f, -dt });
		if (wnd.kbd.keyIsPressed('D'))
			cam.translate({ dt, 0.0f, 0.0f });
		if (wnd.kbd.keyIsPressed('R'))
			cam.translate({ 0.0f, dt, 0.0f });
		if (wnd.kbd.keyIsPressed('F'))
			cam.translate({ 0.0f, -dt, 0.0f });
	}

	while (const auto delta = wnd.mouse.readRawDelta())
		if (!wnd.isCursorEnabled())
			cam.rotate((float)delta->x, (float)delta->y);

	// imgui windows
	cam.spawnControlWindow();
	light.spawnControlWindow();
	showImguiDemoWindow();
	nano.showWindow("Model 1");
	nano2.showWindow("Model 2");
	plane.spawnControlWindow(wnd.gfx());
	//present
	wnd.gfx().endFrame();
}

void App::showImguiDemoWindow()
{
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}