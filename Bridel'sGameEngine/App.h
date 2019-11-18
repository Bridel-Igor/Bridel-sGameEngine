#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();
	// master frame / message loop
	int go();
	~App();
private:
	void doFrame();
private:
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 280;
	
	int wheelRoll = 0, x = 0, y = 0;//
	bool mouseInClient = false;//
	std::string word;//
};