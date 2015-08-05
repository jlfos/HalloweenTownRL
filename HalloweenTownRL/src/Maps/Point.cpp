/*
 * Point.cpp
 *
 *  Created on: Jul 26, 2015
 */

#include "Point.hpp"

Point::Point(int x, int y) : x(x), y(y) {


}

Point::~Point() {
	// TODO Auto-generated destructor stub
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



bool Point::operator !=(const Point& rhs) {
	if(this->x != rhs. x || this->y != rhs.y)
		return true;
	else
		return false;

}
