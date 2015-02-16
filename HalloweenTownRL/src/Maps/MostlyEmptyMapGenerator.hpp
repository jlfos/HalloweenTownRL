/*
 * MostlyEmptyMapGenerator.hpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#ifndef MOSTLYEMPTYMAPGENERATOR_HPP_
#define MOSTLYEMPTYMAPGENERATOR_HPP_

class Map;

class MostlyEmptyMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(Map* map);
	private:
		void AddItem(Map* map, int x, int y);
};


#endif /* MOSTLYEMPTYMAPGENERATOR_HPP_ */
