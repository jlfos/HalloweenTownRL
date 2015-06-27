#include <iostream>
#include "Actor.hpp"
#include "ActorFactory.hpp"
#include "../Ai/Ai.hpp"
#include "../Ai/MonsterAi.hpp"
#include "../Ai/PlayerAi.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Destructible/PlayerDestructible.hpp"
#include "../Destructible/MonsterDestructible.hpp"
#include "../Pickable/Pickable.hpp"
#include "../Pickable/Healer.hpp"
#include "../Tile/TileCharacters.hpp"
#include "../Tile/TileColors.hpp"

Actor *ActorFactory::CreateHero(int x, int y){
	try{
		Actor *player = new Actor(x, y, TileCharacters::Default::AT_SIGN,"player",TileColors::white);
		player->destructible=new PlayerDestructible(30,2,"your cadaver");
		player->attacker=new Attacker(3);
		player->ai = new PlayerAi();
		player->container= new Container(26);
		return player;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateHero" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateVampire(int x, int y, EnemyDifficulty difficulty){
	try{
		Actor *vampire;
		if(difficulty == EnemyDifficulty::MEDIUM){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "Hobo vampire", TileColors::lightestRed);
			vampire->destructible = new MonsterDestructible(15, 1, 6, "pile of ash");
			vampire->attacker = new Attacker(3);
			vampire->ai = new MonsterAi();

		}
		if(difficulty == EnemyDifficulty::HARD){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "Techno vampire", TileColors::lightestRed);
			vampire->destructible = new MonsterDestructible(30, 2, 12, "pile of ash");
			vampire->attacker = new Attacker(4);
			vampire->ai = new MonsterAi();

		}
		else if(difficulty == EnemyDifficulty::VERY_HARD){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "Gangster vampire", TileColors::lighterRed);
			vampire->destructible = new MonsterDestructible(45, 3, 24, "pile of ash");
			vampire->attacker = new Attacker(8);
			vampire->ai = new MonsterAi();
		}
		else if(difficulty == EnemyDifficulty::INSANE){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "Flying vampire", TileColors::lightRed);
			vampire->destructible = new MonsterDestructible(80, 4, 48, "pile of ash");
			vampire->attacker = new Attacker(16);
			vampire->ai = new MonsterAi();
		}
		else if(difficulty == EnemyDifficulty::NIGHTMARE){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "Count", TileColors::darkestRed);
			vampire->destructible = new MonsterDestructible(120, 6, 96, "pile of ash");
			vampire->attacker = new Attacker(20);
			vampire->ai = new MonsterAi();
		}
		return vampire;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateVampire" << std::endl;
		throw 0;
	}

}


Actor *ActorFactory::CreateGremlin(int x, int y){
	try{
		Actor *orc = new Actor( x, y, TileCharacters::Default::G_LOWERCASE, "gremlin",
			TCODColor::desaturatedGreen);
		orc->destructible = new MonsterDestructible(6,0, 3,"dead gremlin");
		orc->attacker = new Attacker(1);
		orc->ai = new MonsterAi();
		return orc;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateOrc" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateMedkit(int x, int y){
	try{
		Actor *healthPotion = new Actor(x, y, TileCharacters::Default::PLUS, "medkit",
				TileColors::red);
		healthPotion->blocks = false;
		healthPotion->pickable = new Healer(4);
		return healthPotion;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreatePotion" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateMonster(int x, int y, EnemyDifficulty difficulty, MapType mapType){
	try{
		Actor *monster = nullptr;
		switch(mapType){
			case MapType::CITY:
				monster = CreateMonsterForCity(x, y, difficulty);
				break;
			case MapType::ROAD:
				break;
			case MapType::WOODS:
				break;
		}
		return monster;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateMonster" << std::endl;
	}
}

Actor *ActorFactory::CreateMonsterForCity(int x, int y, EnemyDifficulty difficulty){
	try{
		Actor *monster = nullptr;
		switch(difficulty){
			case EnemyDifficulty::EASY:
				monster = CreateGremlin(x, y);
				break;
			case EnemyDifficulty::MEDIUM:
			case EnemyDifficulty::HARD:
			case EnemyDifficulty::VERY_HARD:
			case EnemyDifficulty::INSANE:
			case EnemyDifficulty::NIGHTMARE:
				monster = CreateVampire(x, y, difficulty);
				break;
		}
		return monster;
	}
	catch(...){
		std::cerr << "An error occurred in CreateMonsterForCity" << std::endl;
	}
}
