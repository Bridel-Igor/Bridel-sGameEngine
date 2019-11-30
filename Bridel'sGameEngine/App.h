#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "TestPlane.h"
#include "Mesh.h"
#include <set>

class App
{
public:
	App(const std::string& commandLine = "");
	// master frame / message loop
	int go();
	~App();
private:
	void doFrame();
	void showImguiDemoWindow();
private:
	std::string commandLine;
	bool showDemoWindow = false;
	ImGuiManager imGui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	Model sponza{ wnd.gfx(), "Models\\Sponza\\sponza.obj", 1.0f / 20.0f };
	TestPlane bluePlane{ wnd.gfx(), 6.0, {0.3f, 0.3f, 1.0f, 0.0f} };
	TestPlane redPlane{ wnd.gfx(), 6.0, {1.0f, 0.3f, 0.3f, 0.0f} };
	/*Model gobber{ wnd.gfx(), "Models\\gobber\\GoblinX.obj", 1.0f};
	Model muro{ wnd.gfx(), "Models\\muro\\muro.obj", 10.0f };
	Model wall1{ wnd.gfx(), "Models\\brick_wall\\brick_wall.obj", 12.0f };
	Model wall2{ wnd.gfx(), "Models\\brick_wall\\brick_wall.obj", 12.0f };
	TestPlane tp1{ wnd.gfx(), 12.0 };
	TestPlane tp2{ wnd.gfx(), 12.0 };
	Model nano{ wnd.gfx(), "Models\\nano_textured\\nanosuit.obj", 1.2f };*/
};