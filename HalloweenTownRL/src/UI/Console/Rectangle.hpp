/*
 * Rectangle.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "libtcod.hpp"

class Rectangle{
public:
	Rectangle(int width, int height, TCODColor color);
	int getWidth();
	int getHeight();
	TCODColor getColor();
private:
	int width;
	int height;
	TCODColor color;
};



#endif /* RECTANGLE_HPP */
