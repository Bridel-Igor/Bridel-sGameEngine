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
#include "NormalMapTwerker.h"
#include <shellapi.h>

#pragma comment (lib, "assimp-vc141-mtd.lib")

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(1280, 720, "Bridel'sGameEngine"),
	light(wnd.gfx())
{
	if (this->commandLine != "")
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-objnorm")
		{
			const std::wstring pathInWide = pArgs[2];
			const std::wstring pathOutWide = pArgs[3];
			NormalMapTwerker::flipYAllNormalMapsInObj(std::string(pathOutWide.begin(), pathOutWide.end())
			);
			throw std::runtime_error("Normal map processed succesfully. Just kidding about that whole runtime error thing.");
		}
	}
	/*wall1.setRootTransform(dx::XMMatrixTranslation(-12.0f, 12.0f, 0.0f));
	wall2.setRootTransform(dx::XMMatrixTranslation(12.0f, 12.0f, 0.0f));
	tp1.setPos({ 12.0f, 0.0f, -12.0f });
	tp2.setPos({ -12.0f, 0.0f, -12.0f });
	tp1.setRotation(PI/2.0f, 0, 0);
	tp2.setRotation(PI/2.0f, 0, 0);
	gobber.setRootTransform(dx::XMMatrixTranslation(-9.0f, 16.5f, -4.0f));
	muro.setRootTransform(dx::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	nano.setRootTransform(dx::XMMatrixTranslation(11.0f, 0.0f, -4.0f));*/
	bluePlane.setPos(cam.getPos());
	redPlane.setPos(cam.getPos());
	wnd.gfx().setProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 400.0f));
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
	/*wall1.draw(wnd.gfx());
	wall2.draw(wnd.gfx());
	tp1.draw(wnd.gfx());
	tp2.draw(wnd.gfx());
	nano.draw(wnd.gfx());
	gobber.draw(wnd.gfx());
	muro.draw(wnd.gfx());*/
	sponza.draw(wnd.gfx());
	light.draw(wnd.gfx());

	bluePlane.draw(wnd.gfx());
	redPlane.draw(wnd.gfx());

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
	//gobber.showWindow(wnd.gfx(), "Gobber");
	//muro.showWindow(wnd.gfx(), "Muro");
	//wall1.showWindow(wnd.gfx(), "Wall_1");
	//wall2.showWindow(wnd.gfx(), "Wall_2");
	////tp1.spawnControlWindow(wnd.gfx());
	//nano.showWindow(wnd.gfx(), "Nano");
	sponza.showWindow(wnd.gfx(), "Sponza");
	bluePlane.spawnControlWindow(wnd.gfx(), "Blue Plane");
	redPlane.spawnControlWindow(wnd.gfx(), "Red Plane");
	//present
	wnd.gfx().endFrame();
}

void App::showImguiDemoWindow()
{
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}