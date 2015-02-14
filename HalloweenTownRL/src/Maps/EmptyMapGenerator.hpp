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
		TCODMap* Generate(int width, int height);

	private:
		void AddItem(int x, int y);
};


#endif /* EMPTYMAPGENERATOR_HPP_ */
