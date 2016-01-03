/*
 * Lot.hpp
 *
 *  Created on: Jan 2, 2016
 */

#ifndef LOT_HPP
#define LOT_HPP
#include <vector>

#include "Point.hpp"
#include "../RandomWrapper.hpp"

class Map;
class TCODMap;
class Room;
class Lot {
public:
	enum class LotPosition{ NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST};

	Lot(TCODMap *neighborhoodMap, Map *map, RandomWrapper *randomWrap, LotPosition position);
	~Lot();
	virtual void PopulateLot();
private:
	enum class RoomCheckResult {NO_SPACE_ROOM, NO_SPACE_LOT_0, NO_SPACE_LOT_1 ,SPACE  };
	RandomWrapper *randomWrap;
	LotPosition position;
	Point lotStart;
	Point lotEnd;
	Map *map;
	std::vector<int> furniture;
	int mapWidth;
	int mapHeight;
	TCODMap *neighborhoodMap;
	bool backDoor;

	int GenerateRoom(Room room, TCODColor color, int roomsLeft);
	void DrawNextDoor(Room* ra);
	void DrawDoor(const Point& door);
	void DrawNorthDoor(Point start, Point end);
	void DrawEastDoor(Point start, Point end);
	void DrawSouthDoor(Point start, Point end);
	void DrawWestDoor(Point start, Point end);
	void DrawNorthWindow(Point start, Point end);
	void DrawEastWindow(Point start, Point end);
	void DrawSouthWindow(Point start, Point end);
	void DrawWestWindow(Point start, Point end);
	bool ValidDoor(const int x, const int y);
	bool InvalidRoomCorners(Point start, Point end);
	void GenerateWindows(Room &room, MapGenerator::Orientation side);
	void DrawInterior(Point start, Point end, int character);
	Room* FindNextDoor(Room room);
	Room* FindNextDoor(Room room, MapGenerator::Orientation potential);
	Lot::RoomCheckResult CheckHorizontalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag);
	Lot::RoomCheckResult CheckVerticalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag);

};



#endif /* LOT_HPP */
