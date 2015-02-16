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

		virtual TCODMap* Generate(Map* map) = 0;
};



#endif /* MAPGENERATOR_HPP_ */
