/*
 * NeighborhoodMapGenerator.hpp
 *
 *  Created on: Nov 9, 2015
 */

#ifndef NEIGHBORHOODMAPGENERATOR_HPP
#define NEIGHBORHOODMAPGENERATOR_HPP
#include "MapGenerator.hpp"

class NeighborhoodMapGenerator : public MapGenerator {

private:
	int width;
	int height;
	MapGenerator::Orientation orientation;

public:
	NeighborhoodMapGenerator(int width, int height, MapGenerator::Orientation orientation);
	void PopulateActors(Map* map);
	TCODMap* Generate(Map* map, bool generateActors);
};



#endif /* NEIGHBORHOODMAPGENERATOR_HPP */
