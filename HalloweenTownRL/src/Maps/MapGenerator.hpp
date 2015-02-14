/*
 * MapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */

#include "libtcod.h";

#ifndef MAPGENERATOR_HPP_
#define MAPGENERATOR_HPP_

class MapGenerator{
	public:

		virtual TCODMap* Generate(int width, int height) = 0;
};



#endif /* MAPGENERATOR_HPP_ */
