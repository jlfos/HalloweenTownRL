/*
 * NeighborhoodMapGenerator.hpp
 *
 *  Created on: Nov 9, 2015
 */

#ifndef NEIGHBORHOODMAPGENERATOR_HPP
#define NEIGHBORHOODMAPGENERATOR_HPP
#include "libtcod.hpp"
#include "MapGenerator.hpp"
#include "Point.hpp"
#include "Room.hpp"
#include "../RandomWrapper.hpp"
class NeighborhoodMapGenerator : public MapGenerator {

private:
	int mapWidth;
	int mapHeight;
	int minRoomSizeX;
	int minRoomSizeY;
	int maxRoomSizeX;
	int maxRoomSizeY;
	MapGenerator::Orientation flagOri;
	Map* map;
	TCODMap* neighborhoodMap;
public:
	NeighborhoodMapGenerator(int width, int height, MapGenerator::Orientation orientation);
	void PopulateActors(Map* map);
	TCODMap* Generate(Map* map, bool generateActors);

private:
	RandomWrapper randomWrap;
	int GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft);
	void GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, int roomsLeft);
	void DrawNorthWall(Point start, Point end, TCODColor color);
	void DrawSouthWall(Point start, Point end, TCODColor color);
	void DrawEastWall(Point start, Point end, TCODColor color);
	void DrawWestWall(Point start, Point end, TCODColor color);
	void DrawInterior(Point start, Point end, int character);
	void DrawNECorner(Point point, TCODColor color);
	void DrawSECorner(Point point, TCODColor color);
	void DrawSWCorner(Point point, TCODColor color);
	void DrawNWCorner(Point point, TCODColor color);
	void DrawDoor(const Point& door);
	void DrawFence(Point start, Point end);
	void EraseFence(Point start, Point end);
	void DrawNorthDoor(Point start, Point end);
	void DrawEastDoor(Point start, Point end);
	void DrawSouthDoor(Point start, Point end);
	void DrawWestDoor(Point start, Point end);
	Room* FindNextDoor(Point start, Point end);
	Room* FindNextDoor(Point start, Point end, Orientation potential);
	void GenerateRoom(const Room& room, TCODColor color, int roomsLeft);
	void DrawNextDoor(Room* ra);
	void DrawWalls(Orientation previousOrientation, const Room& room, TCODColor color);
	void CreateHouse(int lotX, int lotY, TCODColor visible);
	void DrawRoad(int x, int y, TCODMap* roadMap);
	void DrawGrass(int x, int y, TCODMap* roadMap);
};



#endif /* NEIGHBORHOODMAPGENERATOR_HPP */
