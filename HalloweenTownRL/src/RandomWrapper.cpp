/*
 * RandomWrapper.cpp
 *
 *  Created on: Sep 6, 2015
 */

#include "RandomWrapper.hpp"

#include <mersenne.hpp>
#include <initializer_list>
#include <iostream>

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

RandomWrapper::~RandomWrapper() {
	if(rng!=nullptr){
		delete rng;
	}
	rng = nullptr;
}

int RandomWrapper::getInt(int min, int max) {
	try{
		if(min>max){
			std::cerr << "Min cannot be large then max" << std::endl;
			throw 0;
		}
		else
			return rng->getInt(min, max);

	}
	catch(...){
		std::cerr << "An error occurred in RandomWrapper::getInt" << std::endl;
		throw 0;
	}
}
