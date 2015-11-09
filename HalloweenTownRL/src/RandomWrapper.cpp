/*
 * RandomWrapper.cpp
 *
 *  Created on: Sep 6, 2015
 */

#include "RandomWrapper.hpp"

#include <mersenne.hpp>
#include <initializer_list>

#include "LoggerWrapper.hpp"

#ifndef RW_LOGGER
//#define RW_LOGGER
#endif

RandomWrapper::RandomWrapper() {

	rng = std::unique_ptr<TCODRandom>(TCODRandom::getInstance());
	data = {MapGenerator::Orientation::NORTH,
			MapGenerator::Orientation::SOUTH,
			MapGenerator::Orientation::SOUTH,
			MapGenerator::Orientation::EAST,
			MapGenerator::Orientation::WEST };

	indexO = 0;//data.size();
	indexI = 0;
	dataO = {2, 2, 0, 2, 2, 3, 1, 0, 0, 0, 2, 0, 2, 2, 3, 0, 3, 0, 1};
	dataI = {6, 6, 8, 5, 8, 5, 5, 6, 5, 5, 6 ,5};

}

MapGenerator::Orientation RandomWrapper::GetOrientation() {
	if(indexO < data.size()){

		MapGenerator::Orientation  orientation = data.at(indexO);
#ifdef RW_LOGGER
		LoggerWrapper::Debug ("not rand ori " + std::to_string(orientation));
#endif
		indexO++;
		return orientation;
	}
	else{
		MapGenerator::Orientation  orientation = (MapGenerator::Orientation)rng->getInt(0,3);
#ifdef RW_LOGGER
		LoggerWrapper::Debug ("ori rand" + std::to_string(orientation));
#endif
		return orientation;
	}
}

RandomWrapper::~RandomWrapper() {

}

int RandomWrapper::getInt(int min, int max) {
	try{
		if(min>max){
			LoggerWrapper::Error("Min " + std::to_string(min) + " cannot be larger then max " + std::to_string(max));
			throw 0;
		}
		else{
			int temp = rng->getInt(min, max);
			return temp;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RandomWrapper::getInt");
		throw 0;
	}
}


int RandomWrapper::getInt(int min, int max, bool test) {
	try{
		if(min>max){
			LoggerWrapper::Error("Min cannot be larger then max");
			throw 0;
		}
		else{
			int temp;
			if(indexI < dataI.size()){
				temp = dataI.at(indexI);
				indexI++;
			}
			else{
				temp = rng->getInt(min, max);
			}
#ifdef RW_LOGGER
			LoggerWrapper::Debug("int rand " + std::to_string(temp));
#endif
			return temp;

		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RandomWrapper::getInt");
		throw 0;
	}
}
