#include "math.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "../Actor/Actor.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "../Tile/TileColors.hpp"

const float fogOfWarModifer = 0.1;
const float edgeOfVision = 0.2;

Map::Map(int width, int height) :
		width(width), height(height), tiles(width * height, Tile()) {
	try{
		map = nullptr;
		rng = nullptr;
		lastSeen = nullptr;
		generator = nullptr;
		seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
		actors = new TCODList<Actor>();
	}
	catch(...){
		std::cerr << "An error occurred with Map::Map(int, int)"  << std::endl;
		throw 0;
	}
}

Map::Map(int width, int height, MapGenerator* generator):
		width(width), height(height), generator(generator), tiles(width * height, Tile())
		{
	try{
		map = nullptr;
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

		if(generator == nullptr){
			throw 0;
		}
		else if(map == nullptr){
			map = generator->Generate(this, true);
			generator->PopulateActors(this);


			for(Actor *actor : actors){
				if(actor->lightsource!=nullptr){
					computeLight(actor, true);
				}
			}
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
		throw 0;
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
			zip.putInt(tiles.at(i).explored);
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
		map = generator->Generate(this, false);
		for (int i = 0; i < width * height; i++) {
			tiles.at(i).explored = zip.getInt();
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
		throw 0;
	}
}

int Map::GetHeight(){
	try{
		return height;
	}
	catch(...){
		std::cerr << "An error occurred in Map::GetHeight" << std::endl;
		throw 0;
	}
}



void Map::SetTileProperties(int tileIndex, TCODColor visible, TCODColor fog, int character){
	try{

		tiles.at(tileIndex).visibleColor = visible;
		tiles.at(tileIndex).fogColor = fog;
		tiles.at(tileIndex).character = character;
	}
	catch(...){
		std::cerr << "An error occurred in Map::SetTileProperties" << std::endl;
		throw 0;
	}

}

bool Map::TileHasBeenSet(int tileIndex){
	try{
		if(tiles.at(tileIndex).character == TileCharacters::Default::RAINBOW)
			return false;
		else
			return true;
	}
	catch(...){
		std::cerr << "An error occurred in Map::TileHasBeenSet" << std::endl;
		throw 0;
	}

}

bool Map::CanWalk(int x, int y) const {
	try{
		TileType type = GetTileType(x, y);
		if(type != TileType::GROUND){// this is a wall or edge
			return false;
		}
		for (Actor *actor : engine.actors) {
			if (actor->blocks && actor->x == x && actor->y == y) {// there is a blocking actor here. cannot walk
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
		return tiles.at(x + y * width).explored;
	}
	catch(...){
		std::cerr << "An error occurred with Map::IsExplored"  << std::endl;
		throw 0;
	}
}

bool Map::IsInFov(int x, int y) {
	try{

		if (x < 0 || x >= width || y < 0 || y >= height) { //position is off of the map
			return false;
		}

		if (map->isInFov(x, y) && tiles.at(x + y * width).lit && tiles.at(x + y * width).visibility > edgeOfVision) { //position is in fov, currently lit & has a visibility of 20%
			tiles.at(x + y * width).explored = true;
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

void Map::Render() {
	try{
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {


				 if (IsInFov(x, y)) {
					engine.gui->setCharAdjusted(x, y, tiles.at(x + y * width).character);
					engine.gui->setForegroundAdjusted(x, y, tiles.at(x + y * width).visibleColor * tiles.at(x + y * width).visibility);
				}
				else if (IsExplored(x, y)) {
					engine.gui->setCharAdjusted(x, y, tiles.at(x + y * width).character);
					engine.gui->setForegroundAdjusted(x, y, tiles.at(x + y * width).visibleColor * fogOfWarModifer);
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
		if(owner){ //checks to see if owner is null
			if(owner->lightsource){ // checks to see if lightsource is null
				int centerX = owner->x;
				int centerY = owner->y;
				int startX = std::max(centerX - radius,0);
				int startY = std::max(centerY - radius,0);
				int diameter = radius * 2;
					for(int y=0, pointY = startY; y < diameter && y < height ; y++, pointY++){
						for(int x = 0, pointX = startX; x < diameter && x < width ;x++, pointX++ ){
							float distance = sqrt((centerX-pointX)*(centerX-pointX) + (centerY-pointY)*(centerY-pointY)) ;
							unsigned int tileIndex = pointX + pointY* width;
							if(distance < radius &&  tileIndex < tiles.size() ){ //checks to see if current tile distance is within the radius
								if(isVisible){  // if visible then modify the visibility of the tile
									tiles.at(tileIndex).lit = isVisible;
									float distanceRatio = (1.0 - distance/radius);
									tiles.at(tileIndex).visibility = std::max(distanceRatio , tiles.at(tileIndex).visibility); //we want to use the highest visibility value
								}
								else{ //if not visible then set values accordingly
									tiles.at(tileIndex).lit = false;
									tiles.at(tileIndex).visibility = 0;
								}
							}
						}
					}
			}
			else{
				std::cerr << "Lightsource cannot be null" << std::endl;
				throw 0;
			}
		}
		else{
			std::cerr << "Owner cannot be null" << std::endl;
			throw 0;
		}
	}
	catch(...){
		std::cerr << "An error occurred in Map::computeLight"  << std::endl;
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
		std::cerr << "An error occurred in Map::computeLight"  << std::endl;
		throw 0;
	}
}

void Map::computeNonplayerLights(){
	try{

		for(Actor* actor : actors){
			if(actor!=engine.player && actor->lightsource)
				computeLight(actor, true);
		}
		computeLight(engine.player, true);

	}
	catch(...){
		std::cerr << "An error occurred in Map::computeLights"  << std::endl;
		throw 0;
	}
}

float Map::getTileVisibility(int x, int y){
	try{
		return tiles.at(x + y * width).visibility;
	}
	catch(...){
		std::cerr << "An error occurred in Map::getTileVisibility" << std::endl;
		throw 0;
	}
}
