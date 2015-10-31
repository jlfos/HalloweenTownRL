/*
 * RandomWrapper.hpp
 *
 *  Created on: Sep 6, 2015
 */

#ifndef RANDOMWRAPPER_HPP
#define RANDOMWRAPPER_HPP
#include <vector>
#include "libtcod.hpp"
#include <memory>

#include "Maps/MapGenerator.hpp"

class RoadMapGenerator;

class RandomWrapper {

	public:
		RandomWrapper();
		~RandomWrapper();
		MapGenerator::Orientation GetOrientation();
		int getInt(int min, int max);
	private:
		std::unique_ptr<TCODRandom> rng;
		int index;
		std::vector<MapGenerator::Orientation> data;


};


#endif /* RANDOMWRAPPER_HPP */
