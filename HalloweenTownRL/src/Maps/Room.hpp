/*
 * Room.hpp
 *
 *  Created on: Sep 7, 2015
 */

#ifndef ROOM_HPP
#define ROOM_HPP

#include "Generators/MapGenerator.hpp"
#include "Point.hpp"


class Room {
public:
	Room(Point nwCorner, Point seCorner, MapGenerator::Orientation orientation);
	Room(Point nwCorner, int offsetX, int offsetY, MapGenerator::Orientation orientation);
	MapGenerator::Orientation getOrientation() const;
	const Point& getNWCorner() const;
	const Point& getNECorner() const;
	const Point& getSECorner() const;
	const Point& getSWCorner() const;

private:
	Point nwCorner;
	Point seCorner;
	MapGenerator::Orientation orientation;



};



#endif /* ROOM_HPP */
