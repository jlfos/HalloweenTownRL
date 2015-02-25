/*
 * EmptyMapGenerator.hpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */

#ifndef EMPTYMAPGENERATOR_HPP_
#define EMPTYMAPGENERATOR_HPP_



class EmptyMapGenerator : public MapGenerator{

	public:
		EmptyMapGenerator();
		EmptyMapGenerator(int seed);
		TCODMap* Generate(Map* map, bool generateActors);

	private:
		void AddItem(Map* map, int x, int y);
		long seed;
};


#endif /* EMPTYMAPGENERATOR_HPP_ */
