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
		void GenerateRoad(int x, int y, int width, TCODMap* roadMap);
		void GenerateGrass(int x, int y, int width, TCODMap* roadMap);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation flagOri;
		Map* map;
		RandomWrapper randomWrap;
		void GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft);
		void GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, int roomsLeft);
		void GenerateNorthWall(Point start, Point end, TCODColor color);
		void GenerateSouthWall(Point start, Point end, TCODColor color);
		void GenerateEastWall(Point start, Point end, TCODColor color);
		void GenerateWestWall(Point start, Point end, TCODColor color);
		void GenerateInterior(Point start, Point end, int character);
		void GenerateNECorner(Point point, TCODColor color);
		void GenerateSECorner(Point point, TCODColor color);
		void GenerateSWCorner(Point point, TCODColor color);
		void GenerateNWCorner(Point point, TCODColor color);
		void GenerateDoor(const Point& door);
		void GenerateFence(Point start, Point end);
		void GenerateNorthDoor(Point start, Point end);
		void GenerateEastDoor(Point start, Point end);
		void GenerateSouthDoor(Point start, Point end);
		void GenerateWestDoor(Point start, Point end);
		Room* FindNextDoor(Point start, Point end);
		Room* FindNextDoor(Point start, Point end, Orientation potential);

};


#endif /* HALLOWEENTOWN_ROADMAPGENERATOR_HPP */
