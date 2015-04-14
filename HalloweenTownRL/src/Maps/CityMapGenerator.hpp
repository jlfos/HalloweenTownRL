/*
 * CityMapGenerator.hpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */
#include <vector>

using namespace std;

#ifndef CITYMAPGENERATOR_HPP_
#define CITYMAPGENERATOR_HPP_

class Map;

class CityMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(Map* map, bool generateActors);
		CityMapGenerator();
		~CityMapGenerator();
		void PopulateActors(Map* map);
	private:
		TCODRandom* rng;
		void CreateBuilding(Map* map, TCODMap* cityMap, int x, int y);
		int GenerateBuildingCharacter(int startX, int startY, int currentX, int currentY, int sizeX, int sizeY);
		void GenerateBuildingColors(TCODColor& visible, TCODColor& fog);
		int eastWestStreet;
		int northSouthStreet;
};



#endif /* CITYMAPGENERATOR_HPP_ */
