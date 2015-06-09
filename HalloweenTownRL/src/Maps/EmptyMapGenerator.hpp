/*
 * EmptyMapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 *
 */

#ifndef HALLOWEENTOWN_EMPTYMAPGENERATOR_HPP
#define HALLOWEENTOWN_EMPTYMAPGENERATOR_HPP



class EmptyMapGenerator : public MapGenerator{

	public:
		EmptyMapGenerator();
		EmptyMapGenerator(int seed);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		void AddItem(Map* map, int x, int y);
		long seed;
};


#endif /* HALLOWEENTOWN_EMPTYMAPGENERATOR_HPP */
