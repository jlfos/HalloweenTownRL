/*
 * MapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */

#include "libtcod.h";

#ifndef MAPGENERATOR_HPP_
#define MAPGENERATOR_HPP_
class Map;

class MapGenerator{
	public:
		enum class Orientation {EAST, WEST, NORTH, SOUTH };
		virtual TCODMap* Generate(Map* map, bool generateActors) = 0;
		virtual void PopulateActors(Map* map) = 0;

};



#endif /* MAPGENERATOR_HPP_ */
