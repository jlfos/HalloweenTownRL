/*
 * RandomWrapper.cpp
 *
 *  Created on: Sep 6, 2015
 */
#include "RandomWrapper.hpp"

RandomWrapper::RandomWrapper() {

	rng = TCODRandom::getInstance();
	data = {MapGenerator::Orientation::NORTH,
			MapGenerator::Orientation::SOUTH,
			MapGenerator::Orientation::SOUTH,
			MapGenerator::Orientation::EAST,
			MapGenerator::Orientation::WEST };

	index = 0;//data.size();
}

MapGenerator::Orientation RandomWrapper::GetOrientation() {
	if(index > data.size()){
		MapGenerator::Orientation  orientation = data.at(index);
		index++;
		return orientation;
	}
	else{
		MapGenerator::Orientation  orientation = (MapGenerator::Orientation)rng->getInt(0,3);
		return orientation;
	}
}
