/*
 * ExampleMapGenerator.hpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#ifndef EXAMPLEMAPGENERATOR_HPP_
#define EXAMPLEMAPGENERATOR_HPP_
class Map;

class ExampleMapGenerator : public MapGenerator{
	public:
		TCODMap* Generate(Map* map, bool generateActors);
		 void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
		 void dig(int x1, int y1, int x2, int y2);
		 void addMonster(int x, int y);
		 void addItem(int x, int y);
		 ExampleMapGenerator();
		 bool canWalk(int x, int y) const;
	private:
		friend class BspListener;
		TCODRandom* rng;
		TCODMap* map;
};



#endif /* EXAMPLEMAPGENERATOR_HPP_ */
