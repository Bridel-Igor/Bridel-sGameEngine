#include "Mouse.h"
#include "WindowsInclude.h"

std::pair<int, int> Mouse::getPos() const noexcept
{
	return{ x, y };
}

std::optional<Mouse::RawDelta> Mouse::readRawDelta() noexcept
{
	if (rawDeltaBuffer.empty())
		return std::nullopt;
	const RawDelta d = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return d;
}

int Mouse::getPosX() const noexcept
{
	return x;
}

int Mouse::getPosY() const noexcept
{
	return y;
}

bool Mouse::isMouseInWindow() const noexcept
{
	return mouseIsInWindow;
}

bool Mouse::isLeftPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::isRightPressed() const noexcept
{
	return rightIsPressed;
}

Mouse::Event Mouse::read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Mouse::Event();
}

void Mouse::clear() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::enableRaw() noexcept
{
	rawEnabled = true;
}

void Mouse::disableRaw() noexcept
{
	rawEnabled = false;
}

bool Mouse::isRawEnabled() const noexcept
{
	return rawEnabled;
}

void Mouse::onMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	trimBuffer();
}

void Mouse::onMouseLeave() noexcept
{
	mouseIsInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	trimBuffer();
}

void Mouse::onMouseEnter() noexcept
{
	mouseIsInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	trimBuffer();
}

void Mouse::onRawDelta(int dx, int dy) noexcept
{
	rawDeltaBuffer.push({ dx, dy });
	trimRawInputBuffer();
}

void Mouse::onLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	trimBuffer();
}

void Mouse::onLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	trimBuffer();
}

void Mouse::onRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	trimBuffer();
}

void Mouse::onRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	trimBuffer();
}

void Mouse::onWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	trimBuffer();
}

void Mouse::onWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	trimBuffer();
}

void Mouse::trimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
		buffer.pop();
}

void Mouse::trimRawInputBuffer() noexcept
{
	while (rawDeltaBuffer.size() > bufferSize)
		rawDeltaBuffer.pop();
}

void Mouse::onWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		onWheelUp(x, y);
	}
	while (wheelDeltaCarry <= - WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		onWheelDown(x, y);
	}
}