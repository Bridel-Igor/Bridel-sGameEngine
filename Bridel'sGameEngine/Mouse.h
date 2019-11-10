#pragma once
#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool isLeftPressed;
		bool isRightPressed;
		int x;
		int y;
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			isLeftPressed(false),
			isRightPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			isLeftPressed(parent.leftIsPressed),
			isRightPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		bool isValid() const noexcept
		{
			return type != Type::Invalid;
		}
		Type getType() const noexcept
		{
			return type;
		}
		std::pair<int, int> getPos() const noexcept
		{
			return{ x,y };
		}
		int getPosX() const noexcept
		{
			return x;
		}
		int getPosY() const noexcept
		{
			return y;
		}
		bool leftIsPressed() const noexcept
		{
			return isLeftPressed;
		}
		bool rightIsPressed() const noexcept
		{
			return isRightPressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> getPos() const noexcept;
	int getPosX() const noexcept;
	int getPosY() const noexcept;
	bool isMouseInWindow() const noexcept;
	bool isLeftPressed() const noexcept;
	bool isRightPressed() const noexcept;
	Mouse::Event read() noexcept;
	bool isEmpty() const noexcept
	{
		return buffer.empty();
	}
	void clear() noexcept;
private:
	void onMouseMove(int x, int y) noexcept;
	void onMouseLeave() noexcept;
	void onMouseEnter() noexcept;
	void onLeftPressed(int x, int y) noexcept;
	void onLeftReleased(int x, int y) noexcept;
	void onRightPressed(int x, int y) noexcept;
	void onRightReleased(int x, int y) noexcept;
	void onWheelUp(int x, int y) noexcept;
	void onWheelDown(int x, int y) noexcept;
	void trimBuffer() noexcept;
	void onWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool mouseIsInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};