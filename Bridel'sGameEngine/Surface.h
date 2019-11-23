#pragma once
#include "WindowsInclude.h"
#include "ExceptionFinder.h"
#include <string>
#include <assert.h>
#include <memory>
#include "ConditionalNoexcept.h"

class Surface
{
public:
	class Color
	{
	public:
		unsigned int dword;
	public:
		constexpr Color() noexcept : dword()
		{}
		constexpr Color(const Color& col) noexcept
			:
			dword(col.dword)
		{}
		constexpr Color(unsigned int dw) noexcept
			:
			dword(dw)
		{}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
			:
			dword((x<<24u) | (r<<16u) | (g<<8u) | b)
		{}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
			:
			dword((r << 16u) | (g << 8u) | b)
		{}
		constexpr Color(Color col, unsigned char x) noexcept
			:
			Color((x<<24u) | col.dword)
		{}
		Color& operator=(Color color) noexcept
		{
			dword = color.dword;
			return *this;
		}
		constexpr unsigned char getX() const noexcept
		{
			return dword >> 24u;
		}
		constexpr unsigned char getA() const noexcept
		{
			return getX();
		}
		constexpr unsigned char getR() const noexcept
		{
			return (dword >> 16u) & 0xFFu;
		}
		constexpr unsigned char getG() const noexcept
		{
			return (dword >> 8u) & 0xFFu;
		}
		constexpr unsigned char getB() const noexcept
		{
			return dword & 0xFFu;
		}
		void setX(unsigned char x) noexcept
		{
			dword = (dword & 0xFFFFFFu) | (x << 24u);
		}
		void setA(unsigned char a) noexcept
		{
			setX(a);
		}
		void setR(unsigned char r) noexcept
		{
			dword = (dword & 0xFF00FFFFu) | (r << 16u);
		}
		void setG(unsigned char g) noexcept
		{
			dword = (dword & 0xFFFF00FFu) | (g << 8u);
		}
		void setB(unsigned char b) noexcept
		{
			dword = (dword & 0xFFFFFF00u) | b;
		}
	};
public:
	class Exception : public ExceptionFinder
	{
	public:
		Exception(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		const std::string& getNote() const noexcept;
	private:
		std::string note;
	};
public:
	Surface(unsigned int width, unsigned int height) noexcept;
	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& donor) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface();
	void clear(Color fillValue) noexcept;
	void putPixel(unsigned int x, unsigned int y, Color c) noxnd;
	Color getPixel(unsigned int x, unsigned int y) const noxnd;
	unsigned int getWidth() const noexcept;
	unsigned int getHeight() const noexcept;
	Color* getBufferPtr() noexcept;
	const Color* getBufferPtr() const noexcept;
	const Color* getBufferPtrConst() const noexcept;
	static Surface fromFile(const std::string& name);
	void save(const std::string& filename) const;
	void copy(const Surface& src) noxnd;
private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;
private:
	std::unique_ptr<Color[]> pBuffer;
	unsigned int width;
	unsigned int height;
};