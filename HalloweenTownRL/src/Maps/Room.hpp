/*
 * Room.hpp
 *
 *  Created on: Sep 7, 2015
 */

#ifndef ROOM_HPP
#define ROOM_HPP

#include "Map.hpp"
#include "Generators/MapGenerator.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"

class Room : public Rectangle{
public:
	Room(Point nwCorner, Point seCorner, MapGenerator::Orientation orientation);
	Room(Point nwCorner, int offsetX, int offsetY, MapGenerator::Orientation orientation);
	MapGenerator::Orientation getOrientation() const;
	const Point& getNWCorner() const;
	const Point& getNECorner() const;
	const Point& getSECorner() const;
	const Point& getSWCorner() const;
	void Draw(Map* map, bool skipFilledTiles);

	void SetWindowsEast(bool windowsEast);
	void SetWindowsNorth(bool windowsNorth);
	void SetWindowsSouth(bool windowsSouth);
	void SetWindowsWest(bool windowsWest);
	bool GetWindowsEast() const;
	bool GetWindowsNorth() const;
	bool GetWindowsSouth() const;
	bool GetWindowsWest() const;

private:
	bool windowsWest;
	bool windowsEast;
	bool windowsNorth;
	bool windowsSouth;
	Point nwCorner;
	Point seCorner;
	MapGenerator::Orientation orientation;
	void DrawNECorner(Map* map, Point point);
	void DrawSECorner(Map* map, Point point);
	void DrawSWCorner(Map* map, Point point);
	void DrawNWCorner(Map* map, Point point);
	void DrawHorizontalLine(Map* map, Point start, Point end);
	void DrawVerticalLine(Map* map, Point start, Point end);


};



#endif /* ROOM_HPP */
