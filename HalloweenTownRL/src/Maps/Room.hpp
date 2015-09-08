/*
 * Room.hpp
 *
 *  Created on: Sep 7, 2015
 */

#ifndef ROOM_HPP
#define ROOM_HPP

#include "MapGenerator.hpp"
#include "Point.hpp"


class Room {
public:
	Room(Point start, Point end, MapGenerator::Orientation orientation);
	Room(Point start, int offsetX, int offsetY, MapGenerator::Orientation orientation);
	const Point& getEnd() const;
	MapGenerator::Orientation getOrientation() const;
	const Point& getStart() const;

private:
	Point start;
	Point end;
	MapGenerator::Orientation orientation;



};



#endif /* ROOM_HPP */
