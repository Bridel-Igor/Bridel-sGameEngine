#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

class App
{
public:
	App();
	// master frame / message loop
	int go();
	~App();
private:
	void doFrame();
	void spawnSimulationWindow() noexcept;
	void spawnBoxWindowManagerWindow() noexcept;
	void spawnBoxWindows() noexcept;
private:
	ImGuiManager imGui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	static constexpr size_t nDrawables = 180;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
};