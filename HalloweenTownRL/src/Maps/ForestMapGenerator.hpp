/*
 * MostlyEmptyMapGenerator.hpp
 *
 *  Created on: Feb 15, 2015
 *
 */

#ifndef HALLOWEENTOWN_FORESTMAPGENERATOR_HPP
#define HALLOWEENTOWN_FORESTMAPGENERATOR_HPP


#include "libtcod.hpp"
#include "MapGenerator.hpp"

class Map;

class ForestMapGenerator : public MapGenerator{
	public:
		ForestMapGenerator();
		ForestMapGenerator(MapGenerator::Orientation orientation);
		~ForestMapGenerator();
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		TCODRandom* rng;
		void GenerateTile(int x, int y, int width, int height, TCODMap* forestMap, Map* map);
		void GenerateTree(int x, int y, int width, TCODMap* forestMap, Map* map);
		void GenerateGrass(int x, int y, int width, TCODMap* forestMap, Map* map);
		MapGenerator::Orientation orientation;
};


#endif /* HALLOWEENTOWN_MOSTLYEMPTYMAPGENERATOR_HPP */
