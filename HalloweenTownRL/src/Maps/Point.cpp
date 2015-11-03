/*
 * Point.cpp
 *
 *  Created on: Jul 26, 2015
 */

#include <iostream>
#include "Point.hpp"

Point::Point(int x, int y){

	if(x < 0 && y < 0){
		std::cerr << "X and Y are both negative. Negative values are not legal for the Point constructor." << std::endl;
		throw 0;
	}
	else if(x < 0){
		std::cerr << "X is negative. Negative values are not legal for the Point constructor." << std::endl;
		throw 0;
	}
	else if(y < 0){
		std::cerr << "Y is negative. Negative values are not legal for the Point constructor." << std::endl;
		throw 0;
	}
	else{
		this->x = x;
		this->y = y;
	}

}

Point::Point(const Point &p) : Point(p.x, p.y) {
}

Point::~Point() {

}

int Point::getX() const {
	return x;
}

int Point::getY() const {
	return y;
}

bool Point::operator==(const Point& rhs) {
	if(this->x == rhs. x && this->y == rhs.y)
		return true;
	else
		return false;
}

int Point::getTileIndex(int mapWidth) const {
	return x + y * mapWidth;
}

bool Point::operator !=(const Point& rhs) {
	if(this->x != rhs. x || this->y != rhs.y)
		return true;
	else
		return false;

}
