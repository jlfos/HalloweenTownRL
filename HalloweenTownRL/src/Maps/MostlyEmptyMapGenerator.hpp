/*
 * MostlyEmptyMapGenerator.hpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#ifndef MOSTLYEMPTYMAPGENERATOR_HPP_
#define MOSTLYEMPTYMAPGENERATOR_HPP_


class MostlyEmptyMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(int width, int height);
	private:
		void AddItem(int x, int y);
};


#endif /* MOSTLYEMPTYMAPGENERATOR_HPP_ */
