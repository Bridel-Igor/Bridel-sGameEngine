#include "Keyboard.h"

bool Keyboard::keyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

std::optional<Keyboard::Event> Keyboard::readKey() noexcept
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	return {};
}

bool Keyboard::keyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

std::optional<char> Keyboard::readChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	return {};
}

bool Keyboard::charIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::clearKey() noexcept
{
	keybuffer = std::queue<Event>();
}

void Keyboard::clearChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::clear() noexcept
{
	clearKey();
	clearChar();
}

void Keyboard::enableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::disableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::autorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::onKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	trimBuffer(keybuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	trimBuffer(keybuffer);
}

void Keyboard::onChar(char character) noexcept
{
	charbuffer.push(character);
	trimBuffer(charbuffer);
}

void Keyboard::clearState() noexcept
{
	keystates.reset();
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize)
		buffer.pop();
}