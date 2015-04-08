#include "../main.hpp"
#include <iostream>

using namespace std;

Actor *ActorFactory::CreateHero(int x, int y){
	try{
		Actor *player = new Actor(x, y, Actor::CharacterCodes::AT_SIGN,"player",TCODColor::white);
		player->destructible=new PlayerDestructible(30,2,"your cadaver");
		player->attacker=new Attacker(3);
		player->ai = new PlayerAi();
		player->container= new Container(26);
		return player;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreateHero" << endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateVampire(int x, int y, EnemyDifficulty difficulty){
	try{
		Actor *vampire;
		if(difficulty == EnemyDifficulty::MEDIUM){
			vampire = new Actor(x, y, Actor::CharacterCodes::V_LOWERCASE, "Hobo vampire", TCODColor::lightestRed);
			vampire->destructible = new MonsterDestructible(15, 1, 6, "pile of ash");
			vampire->attacker = new Attacker(3);
			vampire->ai = new MonsterAi();

		}
		if(difficulty == EnemyDifficulty::HARD){
			vampire = new Actor(x, y, Actor::CharacterCodes::V_LOWERCASE, "Techno vampire", TCODColor::lightestRed);
			vampire->destructible = new MonsterDestructible(30, 2, 12, "pile of ash");
			vampire->attacker = new Attacker(4);
			vampire->ai = new MonsterAi();

		}
		else if(difficulty == EnemyDifficulty::VERY_HARD){
			vampire = new Actor(x, y, Actor::CharacterCodes::V_LOWERCASE, "Gangster vampire", TCODColor::lighterRed);
			vampire->destructible = new MonsterDestructible(45, 3, 24, "pile of ash");
			vampire->attacker = new Attacker(8);
			vampire->ai = new MonsterAi();
		}
		else if(difficulty == EnemyDifficulty::INSANE){
			vampire = new Actor(x, y, Actor::CharacterCodes::V_LOWERCASE, "Flying vampire", TCODColor::lightRed);
			vampire->destructible = new MonsterDestructible(80, 4, 48, "pile of ash");
			vampire->attacker = new Attacker(16);
			vampire->ai = new MonsterAi();
		}
		else if(difficulty == EnemyDifficulty::NIGHTMARE){
			vampire = new Actor(x, y, Actor::CharacterCodes::V_LOWERCASE, "Count", TCODColor::darkestRed);
			vampire->destructible = new MonsterDestructible(120, 6, 96, "pile of ash");
			vampire->attacker = new Attacker(20);
			vampire->ai = new MonsterAi();
		}
		return vampire;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreateVampire" << endl;
		throw 0;
	}

}


Actor *ActorFactory::CreateImp(int x, int y){
	try{
		Actor *orc = new Actor( x, y, Actor::CharacterCodes::G_LOWERCASE, "gremlin",
			TCODColor::desaturatedGreen);
		orc->destructible = new MonsterDestructible(6,0, 3,"dead gremlin");
		orc->attacker = new Attacker(1);
		orc->ai = new MonsterAi();
		return orc;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreateOrc" << endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateTroll(int x, int y){
	try{
		Actor *troll = new Actor(x,y, Actor::CharacterCodes::T_LOWERCASE,"troll",
			 TCODColor::darkBlue);
		troll->destructible = new MonsterDestructible(16,1, 0, "troll carcass");
		troll->attacker = new Attacker(4);
		troll->ai = new MonsterAi();
	   return troll;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreateTroll" << endl;
		throw 0;
	}

}

Actor *ActorFactory::CreatePotion(int x, int y){
	try{
		Actor *healthPotion = new Actor(x, y, Actor::CharacterCodes::EXCLAMATION, "health potion",
				TCODColor::violet);
		healthPotion->blocks = false;
		healthPotion->pickable = new Healer(4);
		return healthPotion;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreatePotion" << endl;
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
		cerr << "An error occurred in ActorFactory::CreateMonster" << endl;
	}
}

Actor *ActorFactory::CreateMonsterForCity(int x, int y, EnemyDifficulty difficulty){
	try{
		Actor *monster = nullptr;
		switch(difficulty){
			case EnemyDifficulty::EASY:
				monster = CreateImp(x, y);
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
		cerr << "An error occurred in CreateMonsterForCity" << endl;
	}
}
