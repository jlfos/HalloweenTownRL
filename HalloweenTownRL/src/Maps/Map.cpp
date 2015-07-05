#include "math.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "../Actor/Actor.hpp"
#include "../Engine.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "../Tile/TileColors.hpp"


Map::Map(int width, int height) :
		width(width), height(height) {
	try{
		map = nullptr;
		tiles = nullptr;
		rng = nullptr;
		lastSeen = nullptr;
		seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
		actors = new TCODList<Actor>();
	}
	catch(...){
		std::cerr << "An error occurred with Map::Map(int, int)"  << std::endl;
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
		lastSeen = nullptr;
		seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
		actors = new TCODList<Actor>();
	}
	catch(...){
		std::cerr << "An error occurred with Map::Map(int, int, MapGenerator)"  << std::endl;
		throw 0;
	}

}

void Map::Init() {
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


//			for(Actor *actor : actors){
//				if(actor->lightsource!=nullptr){
//					computeLight(actor, true);
//				}
//			}
		}
	}
	catch(...){
		std::cerr << "An error occurred with Map::Init"  << std::endl;
		throw 0;
	}
}

ActorFactory::EnemyDifficulty Map::GetDifficulty(){
	try{
		ActorFactory::EnemyDifficulty current;
		switch(engine.currentTime.GetHour()){
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
		std::cerr << "An error occurred in Map::GetDifficulty" << std::endl;
	}
}


void Map::PopulateActors(){
	try{
		if(generator)
			generator->PopulateActors(this);
	}
	catch(...){
		std::cerr << "An error occurred in Map::PopulateActors()" << std::endl;
	}
}

void Map::Save(TCODZip &zip) {
	try{
		zip.putInt(seed);
		for (int i = 0; i < width * height; i++) {
			zip.putInt(tiles[i].explored);
		}
		int size = actors.size();
		zip.putInt(size);
		for(Actor *it : actors){
			(it)->Save(zip);
		}
	}
	catch(...){
		std::cerr << "An error occurred with Map::Save"  << std::endl;
		throw 0;
	}
}


Time* Map::TimeLastSeen(){
	return lastSeen;
};

void Map::TimeLastSeen(Time* time){
	lastSeen = time;
}

void Map::Load(TCODZip &zip) {
	try{

		seed = zip.getInt();
		rng = new TCODRandom(seed, TCOD_RNG_MT);
		tiles = new Tile[width * height];
//		generator = new EmptyMapGenerator();
		map = generator->Generate(this, false);
		for (int i = 0; i < width * height; i++) {
			tiles[i].explored = zip.getInt();
		}
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TileColors::white);
			actor->Load(zip);
			actors.push(actor);
			nbActors--;
		}
	}
	catch(...){
		std::cerr << "An error occurred with Map::Load"  << std::endl;
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
		actors.clearAndDelete();
	}
	catch(...){
		std::cerr << "An error occurred with Map::~Map"  << std::endl;
		throw 0;
	}
}

Map::TileType Map::GetTileType(int x, int y) const {
	try{

		TCOD_keycode_t lastKey = engine.lastKey.vk;
		if(!map->isWalkable(x, y)){
			if(y<0 && lastKey == TCODK_UP)
				return TileType::TOP_EDGE;
			else if(x >= width-1 && lastKey == TCODK_RIGHT)
				return TileType::RIGHT_EDGE;
			else if(y >= height-1 && lastKey == TCODK_DOWN)
				return TileType::BOTTOM_EDGE;
			else if(x<0 && lastKey == TCODK_LEFT)
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
		std::cerr << "An error occurred with Map::IsWall"  << std::endl;
		throw 0;
	}
}

int Map::GetWidth(){
	try{
		return width;
	}
	catch(...){
		std::cerr << "An error occurred in Map::GetWidth" << std::endl;
	}
}

int Map::GetHeight(){
	try{
		return height;
	}
	catch(...){
		std::cerr << "An error occurred in Map::GetHeight" << std::endl;
	}
}



void Map::SetTileProperties(int tileIndex, TCODColor visible, TCODColor fog, int character){
	tiles[tileIndex].visibleColor = visible;
	tiles[tileIndex].fogColor = fog;
	tiles[tileIndex].character = character;

}

bool Map::TileHasBeenSet(int tileIndex){
	if(tiles[tileIndex].character == TileCharacters::Default::RAINBOW)
		return false;
	else
		return true;

}

bool Map::CanWalk(int x, int y) const {
	try{
		TileType type = GetTileType(x, y);
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
		std::cerr << "An error occurred with Map::CanWalk"  << std::endl;
		throw 0;
	}
}

bool Map::IsExplored(int x, int y) const {
	try{
		return tiles[x + y * width].explored;
	}
	catch(...){
		std::cerr << "An error occurred with Map::IsExplored"  << std::endl;
		throw 0;
	}
}

bool Map::IsInFov(int x, int y) const {
	try{

		if (x < 0 || x >= width || y < 0 || y >= height) {
			return false;
		}

		if (map->isInFov(x, y)/*&&tiles[x + y * width].lit*/ ) {
			tiles[x + y * width].explored = true;
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Map::IsInFov"  << std::endl;
		throw 0;
	}

}

void Map::ComputeFov()  {
	try{
		map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
	}
	catch(...){
		std::cerr << "An error occurred with Map::ComputeFov"  << std::endl;
		throw 0;
	}
}

void Map::Render() const {
	try{
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {


				 if (IsInFov(x, y)) {
					TCODConsole::root->setChar(x,y, tiles[x+y*width].character);
					TCODConsole::root->setCharForeground(x,y,tiles[x+y*width].visibleColor);
				}
				else if (IsExplored(x, y)) {
					TCODConsole::root->setChar(x,y, tiles[x+y*width].character);
					TCODConsole::root->setCharForeground(x,y,tiles[x+y*width].fogColor);
				}
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred with Map::Render"  << std::endl;
		throw 0;
	}

}

void Map::computeLight(Actor* owner, bool isVisible, int radius){
	try{
		int startX = owner->x;
		int startY = owner->y;
		int width = GetWidth();
		int ewr = radius;
			for(int y=0,pointY= std::max(startY-ewr,0);y<radius*2;y++,pointY++){
				for(int x = 0,pointX= std::max(startX-ewr,0); x<radius*2;x++,pointX++ ){
					float distance = sqrt((startX-pointX)*(startX-pointX) + (startY-pointY)*(startY-pointY)) ;
					if(distance < radius && (pointX + pointY* width) ){
						tiles[pointX + pointY* width].lit = isVisible;
					}


				}
			}
	}
	catch(...){
		std::cerr << "An error occurred with Map::computeLight"  << std::endl;
		throw 0;
	}
}

void Map::computeLight(Actor* owner, bool isVisible){
	try{
		if(owner->lightsource)
			computeLight(owner, isVisible, owner->lightsource->getRadius());
		else{
			std::cerr << "You cannot computeLight on an actor that is not a lightsource" << std::endl;
			throw 0;
		}
	}
	catch(...){
		std::cerr << "An error occurred with Map::computeLight"  << std::endl;
		throw 0;
	}
}

void Map::computeNonplayerLights(){
	try{
		for(Actor* actor : actors){
			if(actor!=engine.player)
				computeLight(actor, true);
		}
		computeLight(engine.player, true);
	}
	catch(...){
		std::cerr << "An error occurred with Map::computeLights"  << std::endl;
		throw 0;
	}
}
