/*
 * RoadMapGenerator.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */

#ifndef HALLOWEENTOWN_ROADMAPGENERATOR_HPP
#define HALLOWEENTOWN_ROADMAPGENERATOR_HPP

#include "libtcod.hpp"
#include "MapGenerator.hpp"

class Map;

class RoadMapGenerator : public MapGenerator{
	public:
		RoadMapGenerator();
		RoadMapGenerator(MapGenerator::Orientation orientation);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		void GenerateTile(int x, int y, int width, int height, TCODMap* roadMap, Map* map);
		void GenerateRoad(int x, int y, int width, TCODMap* roadMap, Map* map);
		void GenerateGrass(int x, int y, int width, TCODMap* roadMap, Map* map);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation orientation;
};


#endif /* HALLOWEENTOWN_ROADMAPGENERATOR_HPP */
