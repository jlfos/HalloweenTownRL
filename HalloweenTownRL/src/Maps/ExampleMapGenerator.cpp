/*
 * ExampleMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: CeilingTileBandit
 */

#include <iostream>
#include "../main.hpp"

using namespace std;


static const int ROOM_MAX_SIZE = 14;
static const int ROOM_MIN_SIZE = 8;
static const int MAX_ROOM_MONSTERS = 4;
static const int MAX_ROOM_ITEMS = 2;


class BspListener: public ITCODBspCallback {
private:
	ExampleMapGenerator &map; // a map to dig
	int roomNum; // room number
	int lastx, lasty; // center of the last room
public:
	BspListener(ExampleMapGenerator &map) :
			map(map), roomNum(0) {
	}
	bool visitNode(TCODBsp *node, void *userData) {
		if (node->isLeaf()) {
			int x, y, w, h;
			// dig a room
			bool withActors = (bool) userData;
			w = map.rng->getInt(ROOM_MIN_SIZE, node->w - 2);
			h = map.rng->getInt(ROOM_MIN_SIZE, node->h - 2);
			x = map.rng->getInt(node->x + 1, node->x + node->w - w - 1);
			y = map.rng->getInt(node->y + 1, node->y + node->h - h - 1);
			map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1,
					withActors);
			if (roomNum != 0) {
				// dig a corridor from last room
				map.dig(lastx, lasty, x + w / 2, lasty);
				map.dig(x + w / 2, lasty, x + w / 2, y + h / 2);
			}
			lastx = x + w / 2;
			lasty = y + h / 2;
			roomNum++;
		}
		return true;
	}
};


ExampleMapGenerator::ExampleMapGenerator(){
	rng = nullptr;
	map = nullptr;
}

TCODMap* ExampleMapGenerator::Generate(Map* map, bool generateActors){
	bool withActors = true;
	long seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
	rng = new TCODRandom(seed, TCOD_RNG_MT);
	TCODMap* tcodMap = new TCODMap(map->width, map->height);
	TCODBsp bsp(0, 0, map->width, map->height);
	bsp.splitRecursive(rng, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, (void *) withActors);
	return tcodMap;
}


void ExampleMapGenerator::createRoom(bool first, int x1, int y1, int x2, int y2,
		bool withActors) {
	try{
		dig(x1, y1, x2, y2);
		if (!withActors) {
			return;
		}

		if (first) {
			// put the player in the first room
			engine.player->x = (x1 + x2) / 2;
			engine.player->y = (y1 + y2) / 2;
		} else {
			TCODRandom *rng = TCODRandom::getInstance();
			int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
			while (nbMonsters > 0) {
				int x = rng->getInt(x1, x2);
				int y = rng->getInt(y1, y2);
				if (canWalk(x, y)) {
					addMonster(x, y);
				}
				nbMonsters--;
			}

			int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
			while (nbItems > 0) {
				int x = rng->getInt(x1, x2);
				int y = rng->getInt(y1, y2);
				if (canWalk(x, y)) {
					addItem(x, y);
				}
				nbItems--;
			}
		}
	}
	catch(...){
		cerr << "An error occurred with ExampleMapGenerator::createRoom"  << endl;
		throw 0;
	}
}


bool ExampleMapGenerator::canWalk(int x, int y) const {
	try{

		if(!map->isWalkable(x,y)){
			// this is a wall or edge
			return false;
		}
		for (Actor *actor : engine.actors) {
			if (actor->blocks && actor->x == x && actor->y == y) {
				// there is a blocking actor here. cannot walk
				return false;
			}
		}
		return true;
	}
	catch(...){
		cerr << "An error occurred with Map::canWalk"  << endl;
		throw 0;
	}
}

void ExampleMapGenerator::addItem(int x, int y) {
	try{
		Actor *healthPotion = ActorFactory::CreatePotion(x, y);
		engine.actors.push(healthPotion);
	}
	catch(...){
		cerr << "An error occurred with ExampleMapGenerator::addItem"  << endl;
		throw 0;
	}
}


void ExampleMapGenerator::dig(int x1, int y1, int x2, int y2) {
	try{
		if (x2 < x1) {

			int tmp = x2;
			x2 = x1;
			x1 = tmp;
		}
		if (y2 < y1) {
			int tmp = y2;
			y2 = y1;
			y1 = tmp;
		}
		for (int tilex = x1; tilex <= x2; tilex++) {
			for (int tiley = y1; tiley <= y2; tiley++) {
				map->setProperties(tilex, tiley, true, true);
			}
		}
	}
	catch(...){
		cerr << "An error occurred with ExampleMapGenerator::dig"  << endl;
		throw 0;
	}
}


void ExampleMapGenerator::addMonster(int x, int y) {
	try{
		TCODRandom *rng = TCODRandom::getInstance();
		if (rng->getInt(0, 100) < 80) {
			// create an orc
			Actor *orc = ActorFactory::CreateOrc(x, y);
			engine.actors.push(orc);
		} else {
			// create a troll
			Actor *troll = ActorFactory::CreateTroll(x, y);

			engine.actors.push(troll);
		}
	}
	catch(...){
		cerr << "An error occurred with ExampleMapGenerator::addMonster"  << endl;
		throw 0;
	}
}
