/*
 * NeighborhoodMapGenerator.hpp
 *
 *  Created on: Nov 9, 2015
 */

#ifndef NEIGHBORHOODMAPGENERATOR_HPP
#define NEIGHBORHOODMAPGENERATOR_HPP
#include "libtcod.hpp"
#include "MapGenerator.hpp"
#include "../Point.hpp"
#include "../Room.hpp"
#include "../../RandomWrapper.hpp"
class NeighborhoodMapGenerator : public MapGenerator {

private:
	u_int mapWidth;
	u_int mapHeight;
	int minRoomSizeX;
	int minRoomSizeY;
	int maxRoomSizeX;
	int maxRoomSizeY;
	int lotSizeX;
	int lotSizeY;
	int lotDesignator;
	bool backDoor;
	MapGenerator::Orientation mapOri;
	Map* map;
	TCODMap* neighborhoodMap;
	int treeChance;
public:
	NeighborhoodMapGenerator(int width, int height, MapGenerator::Orientation orientation);
	void PopulateActors(Map* map);
	TCODMap* Generate(Map* map, bool generateActors);

private:
	enum class RoomCheckResult {NO_SPACE_ROOM, NO_SPACE_LOT_0, NO_SPACE_LOT_1 ,SPACE  };
	RandomWrapper randomWrap;
	int GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft);
	void GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, int roomsLeft);
	void DrawInterior(Point start, Point end, int character);
	void DrawDoor(const Point& door);
	void DrawNorthDoor(Point start, Point end);
	void DrawEastDoor(Point start, Point end);
	void DrawSouthDoor(Point start, Point end);
	void DrawWestDoor(Point start, Point end);
	void DrawNorthWindow(Point start, Point end);
	void DrawEastWindow(Point start, Point end);
	void DrawSouthWindow(Point start, Point end);
	void DrawWestWindow(Point start, Point end);
	Room* FindNextDoor(Room room);
	Room* FindNextDoor(Room room, Orientation potential);
	void GenerateRoom(const Room& room, TCODColor color, int roomsLeft);
	void DrawNextDoor(Room* ra);
	void DrawWalls(Orientation previousOrientation, Room& room, TCODColor color);
	void CreateHouse(int lotX, int lotY, MapGenerator::Orientation side,TCODColor visible);
	bool InvalidRoomCorners(Point start, Point end);
	bool ValidDoor(const int x, const int y);
	NeighborhoodMapGenerator::RoomCheckResult CheckHorizontalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag);
	NeighborhoodMapGenerator::RoomCheckResult CheckVerticalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag);
	void GenerateWindows(Room &room, MapGenerator::Orientation side);

};



#endif /* NEIGHBORHOODMAPGENERATOR_HPP */
