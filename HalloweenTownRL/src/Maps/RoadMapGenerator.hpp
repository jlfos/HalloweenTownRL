/*
 * RoadMapGenerator.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */

#ifndef HALLOWEENTOWN_ROADMAPGENERATOR_HPP
#define HALLOWEENTOWN_ROADMAPGENERATOR_HPP

#include <memory>
#include "libtcod.hpp"
#include "MapGenerator.hpp"
#include "Point.hpp"
#include "../RandomWrapper.hpp"
#include "Room.hpp"
class Map;
class RandomWrapper;

class RoadMapGenerator : public MapGenerator{
	public:
		RoadMapGenerator();
		RoadMapGenerator(MapGenerator::Orientation orientation);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);

	private:
		int minSizeX;
		int minSizeY;
		int maxSizeX;
		int maxSizeY;
		void GenerateTile(int x, int y, int width, int height, TCODMap* roadMap);
		void DrawRoad(int x, int y, int width, TCODMap* roadMap);
		void DrawGrass(int x, int y, int width, TCODMap* roadMap);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation flagOri;
		Map* map;
		TCODMap* roadMap;
		RandomWrapper randomWrap;
		int GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft);
		void GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, int roomsLeft);
		void DrawNorthWall(Point start, Point end, TCODColor color);
		void DrawSouthWall(Point start, Point end, TCODColor color);
		void DrawEastWall(Point start, Point end, TCODColor color);
		void DrawWestWall(Point start, Point end, TCODColor color);
		void DrawInterior(Point start, Point end, int character);
		void DrawHorizontalLine(Point start, Point end, int character, TCODColor color);
		void DrawVerticalLine(Point start, Point end, int character, TCODColor color);
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
};


#endif /* HALLOWEENTOWN_ROADMAPGENERATOR_HPP */
