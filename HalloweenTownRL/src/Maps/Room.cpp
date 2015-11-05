/*
 * Room.cpp
 *
 *  Created on: Sep 7, 2015
 */
#include "Room.hpp"

Room::Room(Point nwCorner, Point seCorner, MapGenerator::Orientation orientation):nwCorner(nwCorner), seCorner(seCorner), orientation(orientation) {

}

Room::Room(Point nwCorner, int offsetX, int offsetY, MapGenerator::Orientation orientation): nwCorner(nwCorner), seCorner(nwCorner.getX() + offsetX, nwCorner.getY() + offsetY), orientation(orientation) {

}


MapGenerator::Orientation Room::getOrientation() const {
	return orientation;
}



const Point& Room::getNECorner() const {
	return Point(seCorner.getX(), nwCorner.getY());
}

const Point& Room::getNWCorner() const {
	return nwCorner;
}

const Point& Room::getSECorner() const {
	return seCorner;
}

const Point& Room::getSWCorner() const {
	return Point(nwCorner.getX(), seCorner.getY());
}
