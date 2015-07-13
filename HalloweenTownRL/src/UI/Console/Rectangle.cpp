/*
 * Rectangle.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include "libtcod.hpp"
#include "Rectangle.hpp"

Rectangle::Rectangle(int width, int height, TCODColor color):
	width(width), height(height), color(color){

}

int Rectangle::getWidth(){
	return width;
}

int Rectangle::getHeight(){
	return height;
}

TCODColor Rectangle::getColor(){
	return color;
}
