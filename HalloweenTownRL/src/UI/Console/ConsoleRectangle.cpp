/*
 * ConsoleRectangle.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include "libtcod.hpp"
#include "ConsoleRectangle.hpp"

ConsoleRectangle::ConsoleRectangle(int width, int height, TCODColor color):
	width(width), height(height), color(color){

}

int ConsoleRectangle::getWidth(){
	return width;
}

int ConsoleRectangle::getHeight(){
	return height;
}

TCODColor ConsoleRectangle::getColor(){
	return color;
}
