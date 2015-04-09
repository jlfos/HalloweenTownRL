/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */



#include "../main.hpp"

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->GetWidth();
	int height = map->GetHeight();
	int enemies = 0;
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {

			if(tilex <= (width/2)+3  &&  tilex >= (width/2)-3){
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*width].visibleColor = TCODColor::lightGrey;
				map->tiles[tilex+tiley*width].fogColor = TCODColor::darkGrey;
				map->tiles[tilex+tiley*width].character = Actor::CharacterCodes::PERIOD;
//				if(enemies==0){
//					enemies++;
//					map->actors.push(ActorFactory::CreateVampire(tilex, tiley, ActorFactory::EnemyDifficulty::MEDIUM));
//				}
//				if(enemies==1){
//					enemies++;
//					map->actors.push(ActorFactory::CreateVampire(tilex, tiley, ActorFactory::EnemyDifficulty::HARD));
//				}
//				if(enemies==2){
//					enemies++;
//					map->actors.push(ActorFactory::CreateVampire(tilex, tiley, ActorFactory::EnemyDifficulty::VERY_HARD));
//				}
//				if(enemies==3){
//					enemies++;
//					map->actors.push(ActorFactory::CreateVampire(tilex, tiley, ActorFactory::EnemyDifficulty::INSANE));
//				}
			}
			else{
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*width].visibleColor = TCODColor::darkGreen;
				map->tiles[tilex+tiley*width].fogColor = TCODColor::darkestGreen;
				map->tiles[tilex+tiley*width].character = Actor::CharacterCodes::PERIOD;
			}
		}
	}

	return emptyMap;
}


void RoadMapGenerator::PopulateActors(Map* map){

}
