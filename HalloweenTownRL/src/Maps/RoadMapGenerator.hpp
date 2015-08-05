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

class Map;

class RoadMapGenerator : public MapGenerator{
	public:
		RoadMapGenerator();
		RoadMapGenerator(MapGenerator::Orientation orientation);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		void GenerateTile(int x, int y, int width, int height, TCODMap* roadMap, Map* map);
		void GenerateRoad(int x, int y, int width, TCODMap* roadMap, Map* map);
		void GenerateGrass(int x, int y, int width, TCODMap* roadMap, Map* map);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation orientation;
		TCODRandom* rng;
		enum Orientation {NORTH = 0, EAST, SOUTH, WEST};
		void GenerateRoom(Map* map, TCODMap* cityMap, Point start, Point end, TCODColor color, Orientation side, int roomsLeft);
		void GenerateNorthWall(Point start, Point end, TCODColor color, Map* map);
		void GenerateSouthWall(Point start, Point end, TCODColor color, Map* map);
		void GenerateEastWall(Point start, Point end, TCODColor color, Map* map);
		void GenerateWestWall(Point start, Point end, TCODColor color, Map* map);
		void GenerateInterior(Point start, Point end, Map* map);
		void GenerateNECorner(Point point, Map* map, TCODColor color);
		void GenerateSECorner(Point point, Map* map, TCODColor color);
		void GenerateSWCorner(Point point, Map* map, TCODColor color);
		void GenerateNWCorner(Point point, Map* map, TCODColor color);
		void GenerateDoor(Point start, Point end, TCODRandom* rng, Map* map, Orientation side);
		int FindNextDoor(Orientation currentDoor);
};


#endif /* HALLOWEENTOWN_ROADMAPGENERATOR_HPP */
