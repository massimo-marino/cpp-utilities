/* 
 * File:   iocolor.h
 * Author: massimo
 *
 * Created on October 17, 2017, 1:50 PM
 */
#ifndef _IOCOLOR_H
#define _IOCOLOR_H

#include <iostream>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////
namespace iocolor
{
	static
	bool
	is_tty (const int fd) noexcept
	{
		return isatty(fd);
	}

	static
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
		black = 0,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,

		none  = -1,
		reset = -2
	};

	enum class effect : uint8_t
	{
		none = 0,

		bold      = 1,  // 1
		underline = 2,  // 4
		blink     = 4,  // 5
		reverse   = 8,  // 7
    concealed = 16, // 8
    strike    = 32  // 9
	};

	inline constexpr
	effect
	operator| (const enum effect a, const enum effect b) noexcept
	{
		return static_cast<effect>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline
	color_definition
	make_color (const enum color fg,
              const enum color bg = color::none,
              const enum effect ef = effect::none) noexcept
	{
		return { static_cast<short>(fg),
		         static_cast<short>(bg),
		         static_cast<uint8_t>(ef) };
	}

	inline
	color_definition
	make_color (const short fg,
              const short bg = -1,
              const uint8_t ef = 0) noexcept
	{
		return { fg, bg, ef };
	}

	inline
	color_definition
	foreground (const enum color fg) noexcept
	{
		return make_color(fg);
	}

	inline
	color_definition
	foreground (const short fg) noexcept
	{
		return make_color(fg);
	}

	inline
	color_definition
	background (const enum color bg) noexcept
	{
		return make_color(color::none, bg);
	}

	inline
	color_definition
	background (const short bg) noexcept
	{
		return make_color(-1, bg);
	}

	inline
	color_definition
	effects (const enum effect ef) noexcept
	{
		return make_color(color::none, color::none, ef);
	}

	inline
	color_definition
	effects (const uint8_t ef) noexcept
	{
		return make_color(-1, -1, ef);
	}
}  // namespace iocolor
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator << (std::ostream& o, const iocolor::color_definition descriptor) noexcept
{
	if ( !iocolor::is_tty(o) )
  {
		return o;
	}

	if (descriptor.effects != 0)
  {
		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::bold))
    {
			o << "\033[1m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::underline))
    {
			o << "\033[4m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::blink))
    {
			o << "\033[5m";
		}

		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::reverse))
    {
			o << "\033[7m";
		}

 		if (descriptor.effects & static_cast<uint8_t>(iocolor::effect::strike))
    {
			o << "\033[9m";
		}
	}

	if (descriptor.foreground != -1)
  {
		if (descriptor.foreground < 8)
    {
			o << "\033[" << descriptor.foreground + 30 << "m";
		}
		else
    {
			o << "\033[38;5;" << descriptor.foreground << "m";
		}
	}

	if (descriptor.background != -1)
  {
		if (descriptor.background < 8)
    {
			o << "\033[" << descriptor.background + 40 << "m";
		}
		else
    {
			o << "\033[48;5;" << descriptor.background << "m";
		}
	}

	return o;
}

std::ostream&
operator << (std::ostream& o, const enum iocolor::color c) noexcept
{
	if (c == iocolor::color::reset)
  {
		return o << "\033[0m";
	}

	return o << iocolor::foreground(c);
}

std::ostream&
operator << (std::ostream& o, const enum iocolor::effect ef) noexcept
{
  if (ef == iocolor::effect::none)
  {
		return o << "\033[0m";
	}

	return o << iocolor::effects(ef);
}

#endif
