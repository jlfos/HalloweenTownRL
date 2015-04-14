/*
 * MostlyEmptyMapGenerator.hpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#ifndef FORESTMAPGENERATOR_HPP_
#define FORESTMAPGENERATOR_HPP_

class Map;

class ForestMapGenerator : public MapGenerator{
	public:
		ForestMapGenerator();
		~ForestMapGenerator();
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		void AddItem(Map* map, int x, int y);
		TCODRandom* rng;
};


#endif /* MOSTLYEMPTYMAPGENERATOR_HPP_ */
