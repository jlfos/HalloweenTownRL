/*
 * Point.cpp
 *
 *  Created on: Jul 26, 2015
 */

#include "../LoggerWrapper.hpp"
#include "Point.hpp"

Point::Point(uint x, uint y){

	if(x < 0 && y < 0){
		LoggerWrapper::Error("X and Y are both negative. Negative values are not legal for the Point constructor.");
		throw 0;
	}
	else if(x < 0){
		LoggerWrapper::Error("X is negative. Negative values are not legal for the Point constructor.");
		throw 0;
	}
	else if(y < 0){
		LoggerWrapper::Error("Y is negative. Negative values are not legal for the Point constructor.");
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

Point::Point(const Point &p, uint x, uint y){
	this->x = p.x + x;
	this->y = p.y + y;
}

uint Point::getX() const {
	return x;
}

uint Point::getY() const {
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

std::string Point::ToString() const {
	std::string ret = "X: " + std::to_string(x);
	ret += " Y: ";
	ret += std::to_string(y);
	return ret;
}
