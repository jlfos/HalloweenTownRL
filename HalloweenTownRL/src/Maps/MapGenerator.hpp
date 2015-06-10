/*
 * MapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 */

#ifndef HALLOWEENTOWN_MAPGENERATOR_HPP
#define HALLOWEENTOWN_MAPGENERATOR_HPP
#include "libtcod.h";

class Map;

class MapGenerator{
	public:
		enum class Orientation {EAST, WEST, NORTH, SOUTH };
		virtual TCODMap* Generate(Map* map, bool generateActors) = 0;
		virtual void PopulateActors(Map* map) = 0;

};



#endif /* HALLOWEENTOWN_MAPGENERATOR_HPP */
