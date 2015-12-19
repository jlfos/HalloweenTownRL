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
	u_int mapWidth;
	u_int mapHeight;
	int minRoomSizeX;
	int minRoomSizeY;
	int maxRoomSizeX;
	int maxRoomSizeY;
	int lotSizeX;
	int lotSizeY;
	MapGenerator::Orientation mapOri;
	Map* map;
	TCODMap* neighborhoodMap;
	int treeChance;
public:
	NeighborhoodMapGenerator(int width, int height, MapGenerator::Orientation orientation);
	void PopulateActors(Map* map);
	TCODMap* Generate(Map* map, bool generateActors);

private:
	enum class BORDERS { T, T_R, T_B, T_L, T_R_B, T_R_B_L};
	RandomWrapper randomWrap;
	int GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft);
	void GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, int roomsLeft);
	void DrawNorthWall(Point start, Point end, TCODColor color);
	void DrawSouthWall(Point start, Point end, TCODColor color);
	void DrawEastWall(Point start, Point end, TCODColor color);
	void DrawWestWall(Point start, Point end, TCODColor color);
	void DrawInterior(Point start, Point end, int character);
	void DrawFilledSquare(Point start, Point end, TCODColor visible,  int character);
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
	void CreateHouse(int lotX, int lotY, MapGenerator::Orientation side,TCODColor visible);
	void DrawRoad(int x, int y, TCODMap* roadMap);
	void DrawSidewalk(int x, int y, TCODMap* roadMap);
	void DrawGrass(int x, int y, TCODMap* roadMap);
	void GenerateTree(int x, int y, TCODMap* forestMap, Map* map);
	bool InvalidRoomCorners(Point start, Point end);
	bool ValidDoor(const int x, const int y);
	Point CheckHorizontalRoom(Point start, bool xNegFlag, bool yNegFlag);
	Point CheckVerticalRoom(Point start, bool xNegFlag, bool yNegFlag);

	void DrawSquareBorders(Point start, Point end, TCODColor visible, BORDERS border, int character);


};



#endif /* NEIGHBORHOODMAPGENERATOR_HPP */
