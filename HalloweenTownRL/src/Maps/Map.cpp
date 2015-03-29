#include <iostream>
#include <string>
#include "../main.hpp"


using namespace std;

static const int ROOM_MAX_SIZE = 14;
static const int ROOM_MIN_SIZE = 8;
static const int MAX_ROOM_MONSTERS = 4;
static const int MAX_ROOM_ITEMS = 2;


Map::Map(int width, int height) :
		width(width), height(height) {
	try{
		map = nullptr;
		tiles = nullptr;
		rng = nullptr;
		seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
		actors = new TCODList<Actor>();
	}
	catch(...){
		cerr << "An error occurred with Map::Map(int, int)"  << endl;
		throw 0;
	}
}

Map::Map(int width, int height, MapGenerator* generator):
		width(width), height(height), generator(generator)
		{
	try{
		map = nullptr;
		tiles = nullptr;
		rng = nullptr;
		seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
		actors = new TCODList<Actor>();
	}
	catch(...){
		cerr << "An error occurred with Map::Map(int, int, MapGenerator)"  << endl;
		throw 0;
	}

}

void Map::init() {
	try{

		if(rng== nullptr)
			rng = new TCODRandom(seed, TCOD_RNG_MT);

		if(tiles == nullptr){
			tiles = new Tile[width * height];
		}

		if(generator == nullptr){
			throw 0;
		}
		else if(map == nullptr){
			map = generator->Generate(this, true);
			generator->PopulateActors(this);
		}
	}
	catch(...){
		cerr << "An error occurred with Map::init"  << endl;
		throw 0;
	}
}

ActorFactory::EnemyDifficulty Map::GetDifficulty(){
	try{
		ActorFactory::EnemyDifficulty current;
		switch(engine.currentTime.getHour()){
			case 6:
			case 7:
				current = ActorFactory::EnemyDifficulty::EASY;
				break;
			case 8:
			case 9:
				current = ActorFactory::EnemyDifficulty::MEDIUM;
				break;
			case 10:
			case 11:
				current = ActorFactory::EnemyDifficulty::HARD;
				break;
			case 12:
			case 1:
				current = ActorFactory::EnemyDifficulty::VERY_HARD;
				break;
			case 2:
			case 3:
				current = ActorFactory::EnemyDifficulty::INSANE;
				break;
			case 4:
			case 5:
				current = ActorFactory::EnemyDifficulty::NIGHTMARE;
				break;
		}
		return current;
	}
	catch(...){
		cerr << "An error occurred in Map::GetDifficulty" << endl;
	}
}


void Map::populateActors(){
	try{
		if(generator)
			generator->PopulateActors(this);
	}
	catch(...){
		cerr << "An error occurred in Map::populateActors()" << endl;
	}
}

void Map::save(TCODZip &zip) {
	try{
		zip.putInt(seed);
		for (int i = 0; i < width * height; i++) {
			zip.putInt(tiles[i].explored);
		}
		int size = actors.size();
		zip.putInt(size);
		for(Actor *it : actors){
			(it)->save(zip);
		}
	}
	catch(...){
		cerr << "An error occurred with Map::save"  << endl;
		throw 0;
	}
}

void Map::load(TCODZip &zip) {
	try{

		seed = zip.getInt();
		rng = new TCODRandom(seed, TCOD_RNG_MT);
		tiles = new Tile[width * height];
		generator = new EmptyMapGenerator();
		map = generator->Generate(this, false);
		for (int i = 0; i < width * height; i++) {
			tiles[i].explored = zip.getInt();
		}
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TCODColor::white);
			actor->load(zip);
			actors.push(actor);
			nbActors--;
		}
	}
	catch(...){
		cerr << "An error occurred with Map::load"  << endl;
		throw 0;
	}
}

Map::~Map() {
	try{

		if(tiles != nullptr){
			delete[] tiles;
			tiles = nullptr;
		}


		if(map != nullptr){
			delete map;
			map = nullptr;
		}

		if(rng != nullptr){
			delete rng;
			rng = nullptr;
		}
//		for(int i = 0; i<actors.size(); i++){
//			delete actors.get(i);
//
//		}
//		for(Actor* actor: actors){
//			if(actor != nullptr){
//				delete actor;
//				actor = nullptr;
//			}
//		}
		actors.clearAndDelete();
	}
	catch(...){
		cerr << "An error occurred with Map::~Map"  << endl;
		throw 0;
	}
}

Map::TileType Map::getTileType(int x, int y) const {
	try{

		if(!map->isWalkable(x, y)){
			if(y<0)
				return TileType::TOP_EDGE;
			else if(x >= width)
				return TileType::RIGHT_EDGE;
			else if(y >= height)
				return TileType::BOTTOM_EDGE;
			else if(x<0)
				return TileType::LEFT_EDGE;
			else{
				return TileType::WALL;
			}
		}
		else{
			return TileType::GROUND;
		}
	}
	catch(...){
		cerr << "An error occurred with Map::isWall"  << endl;
		throw 0;
	}
}

bool Map::canWalk(int x, int y) const {
	try{
		TileType type = getTileType(x, y);
		if(type != TileType::GROUND){
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

bool Map::isExplored(int x, int y) const {
	try{
		return tiles[x + y * width].explored;
	}
	catch(...){
		cerr << "An error occurred with Map::isExplored"  << endl;
		throw 0;
	}
}

bool Map::isInFov(int x, int y) const {
	try{

		if (x < 0 || x >= width || y < 0 || y >= height) {
			return false;
		}

		if (map->isInFov(x, y)) {
			tiles[x + y * width].explored = true;
			return true;
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Map::isInFov"  << endl;
		throw 0;
	}

}

void Map::computeFov() {
	try{
		map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
	}
	catch(...){
		cerr << "An error occurred with Map::computeFov"  << endl;
		throw 0;
	}
}

void Map::render() const {
	try{
		static const TCODColor darkWall(0, 0, 100);
		static const TCODColor darkGround(50, 50, 150);
		static const TCODColor lightWall(130, 110, 50);
		static const TCODColor lightGround(200, 180, 50);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
//				TileType type = getTileType(x, y);
				if (isInFov(x, y)) {
					TCODConsole::root->setChar(x,y, tiles[x+y*width].character);
					TCODConsole::root->setCharForeground(x,y,tiles[x+y*width].visibleColor);
				}
				else if (isExplored(x, y)) {
					TCODConsole::root->setChar(x,y, tiles[x+y*width].character);
					TCODConsole::root->setCharForeground(x,y,tiles[x+y*width].fogColor);
				}
			}
		}
	}
	catch(...){
		cerr << "An error occurred with Map::computeFov"  << endl;
		throw 0;
	}

}
