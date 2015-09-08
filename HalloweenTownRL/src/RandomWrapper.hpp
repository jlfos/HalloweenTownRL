/*
 * RandomWrapper.hpp
 *
 *  Created on: Sep 6, 2015
 */

#ifndef RANDOMWRAPPER_HPP
#define RANDOMWRAPPER_HPP
#include <vector>
#include "libtcod.hpp"
#include "Maps/MapGenerator.hpp"

class RoadMapGenerator;

class RandomWrapper {

	public:
		RandomWrapper();
		MapGenerator::Orientation GetOrientation();
	private:
		TCODRandom* rng;
		int index;
		std::vector<MapGenerator::Orientation> data;


};


#endif /* RANDOMWRAPPER_HPP */
