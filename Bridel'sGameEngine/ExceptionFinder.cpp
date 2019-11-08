#include "ExceptionFinder.h"
#include <sstream>

ExceptionFinder::ExceptionFinder(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* ExceptionFinder::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl << getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ExceptionFinder::getType() const noexcept
{
	return "Exception Finder";
}

int ExceptionFinder::getLine() const noexcept
{
	return line;
}

const std::string& ExceptionFinder::getFile() const noexcept
{
	return file;
}

std::string ExceptionFinder::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}