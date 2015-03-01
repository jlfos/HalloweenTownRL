/*
 * CityMapGenerator.hpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */

#ifndef CITYMAPGENERATOR_HPP_
#define CITYMAPGENERATOR_HPP_

class Map;

class CityMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(Map* map, bool generateActors);
		CityMapGenerator();
	private:
		void CreateBuilding(Map* map, TCODMap* cityMap, int x, int y);
		int eastWestStreet;
		int northSouthStreet;
};



#endif /* CITYMAPGENERATOR_HPP_ */