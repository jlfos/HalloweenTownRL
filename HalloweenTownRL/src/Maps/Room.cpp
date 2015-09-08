/*
 * Room.cpp
 *
 *  Created on: Sep 7, 2015
 */
#include "Room.hpp"

Room::Room(Point start, Point end, MapGenerator::Orientation orientation):start(start), end(end), orientation(orientation) {

}

Room::Room(Point start, int offsetX, int offsetY, MapGenerator::Orientation orientation): start(start), end(start.getX() + offsetX, start.getY() + offsetY), orientation(orientation) {

}

const Point& Room::getEnd() const {
	return end;
}

MapGenerator::Orientation Room::getOrientation() const {
	return orientation;
}

const Point& Room::getStart() const {
	return start;
}
