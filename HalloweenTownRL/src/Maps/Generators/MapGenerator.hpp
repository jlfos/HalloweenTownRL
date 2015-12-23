/*
 * MapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 */

#ifndef HALLOWEENTOWN_MAPGENERATOR_HPP
#define HALLOWEENTOWN_MAPGENERATOR_HPP
#include "libtcod.hpp"
#include "../Point.hpp"

class Map;

class MapGenerator{
	public:
		enum Orientation {NORTH = 0, EAST, SOUTH, WEST, NONE};
		virtual TCODMap* Generate(Map* map, bool generateActors) = 0;
		virtual void PopulateActors(Map* map) = 0;
		void DrawRectangle(Map* map, Point start, Point end, TCODColor, int character);
		void DrawRectangle(Map* map, Point start, Point end, TCODColor, int character, bool skipFilledTiles);
		void DrawHorizontalLine(Map* map, Point start, Point end, TCODColor color, int character);
		void DrawHorizontalLine(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles);
		void DrawVerticalLine(Map* map, Point start, Point end, TCODColor color, int character);
		void DrawVerticalLine(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles);
		void DrawFilledArea(Map* map, Point start, Point end, TCODColor color, int character);
		void DrawFilledArea(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles);
		void DrawTree(Map* map, TCODMap* roadMap, int x, int y);
		void DrawRoad(Map* map, TCODMap* roadMap, int x, int y);
		void DrawWindow(Map* map, TCODMap* roadMap, int x, int y);
		void DrawSidewalk(Map* map, TCODMap* roadMap, int x, int y);
		void DrawGrass(Map* map, TCODMap* roadMap, int x, int y);

};



#endif /* HALLOWEENTOWN_MAPGENERATOR_HPP */
