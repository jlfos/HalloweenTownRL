/*
 * RoadMapGenerator.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */

#ifndef ROADMAPGENERATOR_HPP_
#define ROADMAPGENERATOR_HPP_


class Map;

class RoadMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(Map* map, bool generateActors);
	private:
		void AddItem(Map* map, int x, int y);
};


#endif /* ROADMAPGENERATOR_HPP_ */
