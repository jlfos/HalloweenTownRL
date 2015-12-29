/*
 * Rectangle.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef CONSOLE_RECTANGLE_HPP
#define CONSOLE_RECTANGLE_HPP
#include "libtcod.hpp"

class ConsoleRectangle{
public:
	ConsoleRectangle(int width, int height, TCODColor color);
	int getWidth();
	int getHeight();
	TCODColor getColor();
private:
	int width;
	int height;
	TCODColor color;
};



#endif /* CONSOLE_RECTANGLE_HPP */
