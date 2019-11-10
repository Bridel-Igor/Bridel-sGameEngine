#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::mark()
{
	const steady_clock::time_point old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}