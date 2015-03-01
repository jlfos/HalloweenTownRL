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
		TCODMap* Generate(Map* map, bool generateActors);
	private:
		void AddItem(Map* map, int x, int y);
};


#endif /* MOSTLYEMPTYMAPGENERATOR_HPP_ */
