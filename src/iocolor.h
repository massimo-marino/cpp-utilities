/* 
 * File:   iocolor.h
 * Author: massimo
 *
 * Created on October 17, 2017, 1:50 PM
 */
#ifndef IOCOLOR_H
#define IOCOLOR_H

#include <iostream>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////
namespace iocolor
{
	static inline
	bool
	is_tty (const int fd) noexcept
	{
		return isatty(fd);
	}

	static inline
	bool
	is_tty (const std::ostream& s) noexcept
	{
		if (s.rdbuf() == std::cout.rdbuf())
    {
			return is_tty(STDOUT_FILENO);
		}
		else if (s.rdbuf() == std::cerr.rdbuf())
    {
			return is_tty(STDERR_FILENO);
		}
    return false;
	}

	struct color_definition
	{
		short   foreground;
		short   background;
		uint8_t effects;
	};

	enum class color : short
	{
    reset   = -2, // "\033[0m"
    none    = -1,

    black   = 0,  // "\033[30m"
    red     = 1,  // "\033[31m"
    green   = 2,  // "\033[32m"
    yellow  = 3,  // "\033[33m"
    blue    = 4,  // "\033[34m"
    magenta = 5,  // "\033[35m"
    cyan    = 6,  // "\033[36m"
    white   = 7,  // "\033[37m"
	};

	enum class effect : uint8_t
	{
    none      = 0,  // "\033[0m"

    bold      = 1,  // "\033[1m"
    underline = 2,  // "\033[4m"
    blink     = 4,  // "\033[5m"
    reverse   = 8,  // "\033[7m"
    concealed = 16, // "\033[8m"
    strike    = 32, // "\033[9m"
	};

	inline constexpr
	effect
	operator| (const enum effect a, const enum effect b) noexcept
	{
		return static_cast<effect>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline constexpr
	color_definition
	make_color (const enum color fg,
              const enum color bg = color::none,
              const enum effect ef = effect::none) noexcept
	{
		return { static_cast<short>(fg),
		         static_cast<short>(bg),
		         static_cast<uint8_t>(ef) };
	}

	inline constexpr
	color_definition
	make_color (const short fg,
              const short bg = -1,
              const uint8_t ef = 0) noexcept
	{
		return { fg, bg, ef };
	}

	inline constexpr
	color_definition
	foreground (const enum color fg) noexcept
	{
		return make_color(fg);
	}

	inline constexpr
	color_definition
	foreground (const short fg) noexcept
	{
		return make_color(fg);
	}

	inline constexpr
	color_definition
	background (const enum color bg) noexcept
	{
		return make_color(color::none, bg);
	}

	inline constexpr
	color_definition
	background (const short bg) noexcept
	{
		return make_color(-1, bg);
	}

	inline constexpr
	color_definition
	effects (const enum effect ef) noexcept
	{
		return make_color(color::none, color::none, ef);
	}

	inline constexpr
	color_definition
	effects (const uint8_t ef) noexcept
	{
		return make_color(-1, -1, ef);
	}
}  // namespace iocolor
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator << (std::ostream& os, const iocolor::color_definition& descriptor) noexcept
{
	if ( !iocolor::is_tty(os) )
  {
		return os;
	}

	if (descriptor.effects != 0)
  {
		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::bold))
    {
			os << "\033[1m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::underline))
    {
			os << "\033[4m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::blink))
    {
			os << "\033[5m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::reverse))
    {
			os << "\033[7m";
		}

 		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::concealed))
    {
			os << "\033[8m";
		}

 		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::strike))
    {
			os << "\033[9m";
		}
	}

	if (descriptor.foreground != -1)
  {
		if (descriptor.foreground < 8)
    {
			os << "\033[" << descriptor.foreground + 30 << "m";
		}
		else
    {
			os << "\033[38;5;" << descriptor.foreground << "m";
		}
	}

	if (descriptor.background != -1)
  {
		if (descriptor.background < 8)
    {
			os << "\033[" << descriptor.background + 40 << "m";
		}
		else
    {
			os << "\033[48;5;" << descriptor.background << "m";
		}
	}

	return os;
}

std::ostream&
operator << (std::ostream& os, const enum iocolor::color c) noexcept
{
	if (c == iocolor::color::reset)
  {
		return os << "\033[0m";
	}

	return os << iocolor::foreground(c);
}

std::ostream&
operator << (std::ostream& os, const enum iocolor::effect ef) noexcept
{
  if (ef == iocolor::effect::none)
  {
		return os << "\033[0m";
	}

	return os << iocolor::effects(ef);
}

#endif  // IOCOLOR_H
