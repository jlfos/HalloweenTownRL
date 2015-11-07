#include <iostream>
#include <map>
#include "Actor.hpp"
#include "ActorFactory.hpp"
#include "../Ai/Ai.hpp"
#include "../Destructible/BossDestructible.hpp"
#include "../Ai/MonsterAi.hpp"
#include "../Ai/PlayerAi.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Destructible/PlayerDestructible.hpp"
#include "../Destructible/MonsterDestructible.hpp"
#include "../Lightsource.hpp"
#include "../Item/Item.hpp"
#include "../Item/Healer.hpp"
#include "../Item/MeleeWeapon.hpp"
#include "../Tile/TileCharacters.hpp"
#include "../Tile/TileColors.hpp"

Actor *ActorFactory::CreateHero(int x, int y){
	try{
		Actor *player = new Actor(x, y, TileCharacters::Default::AT_SIGN, "player", TileColors::white);
		player->destructible=new PlayerDestructible(30,2,"your cadaver");
		player->attacker=new Attacker(3, "fists");
		player->ai = new PlayerAi();
		player->container= new Container(26);
		player->lightsource = new Lightsource(10);
		return player;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateHero" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateLampPost(int x, int y){
	try{
		Actor* lampPost = new Actor(x, y, TileCharacters::Default::T_UPPERCASE, "lamp post", TileColors::grey);
		lampPost->blocks = true;
		lampPost->lightsource = new Lightsource(15);
		return lampPost;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateLampPost" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateGiantSpider(int x, int y){
	try{
		Actor *giantSpider = new Actor( x, y, TileCharacters::Default::S_UPPERCASE, "giant spider",
				TCODColor::desaturatedGreen);
		giantSpider->destructible = new BossDestructible(120, 6, 0,"dead giant spider");
		giantSpider->attacker = new Attacker(20);
		giantSpider->ai = new MonsterAi(70);
		return giantSpider;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateGiantSpider" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateVampire(int x, int y, EnemyDifficulty difficulty){
	try{
		Actor *vampire = nullptr;
		if(difficulty == EnemyDifficulty::MEDIUM){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "hobo vampire", TileColors::lightestRed);
			vampire->destructible = new MonsterDestructible(15, 1, 6, "pile of ash");
			vampire->attacker = new Attacker(3);
			vampire->ai = new MonsterAi(35);

		}
		if(difficulty == EnemyDifficulty::HARD){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "techno vampire", TileColors::lightestRed);
			vampire->destructible = new MonsterDestructible(30, 2, 12, "pile of ash");
			vampire->attacker = new Attacker(4);
			vampire->ai = new MonsterAi(35);

		}
		else if(difficulty == EnemyDifficulty::VERY_HARD){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "gangster vampire", TileColors::lighterRed);
			vampire->destructible = new MonsterDestructible(45, 3, 24, "pile of ash");
			vampire->attacker = new Attacker(8);
			vampire->ai = new MonsterAi(35);
		}
		else if(difficulty == EnemyDifficulty::INSANE){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "flying vampire", TileColors::lightRed);
			vampire->destructible = new MonsterDestructible(80, 4, 48, "pile of ash");
			vampire->attacker = new Attacker(16);
			vampire->ai = new MonsterAi(45);
		}
		else if(difficulty == EnemyDifficulty::NIGHTMARE){
			vampire = new Actor(x, y, TileCharacters::Default::V_LOWERCASE, "count", TileColors::darkestRed);
			vampire->destructible = new MonsterDestructible(120, 6, 96, "pile of ash");
			vampire->attacker = new Attacker(20);
			vampire->ai = new MonsterAi(70);
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
		Actor *gremlin = new Actor( x, y, TileCharacters::Default::G_LOWERCASE, "gremlin",
				TCODColor::desaturatedGreen);
		gremlin->destructible = new MonsterDestructible(6,0, 3,"dead gremlin");
		gremlin->attacker = new Attacker(1);
		gremlin->ai = new MonsterAi();
		return gremlin;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateOrc" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateZombie(int x, int y){
	try{
		Actor *zombie = new Actor(x, y, TileCharacters::Default::Z_LOWERCASE, "zombie", TileColors::peach);
		zombie->destructible = new MonsterDestructible(4, 0, 1, "dead zombie");
		zombie->attacker = new Attacker(3);
		zombie->ai = new MonsterAi(10);
		return zombie;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateZombie" << std::endl;
		throw 0;
	}

}

Actor *ActorFactory::CreateMedkit(int x, int y){
	try{
		Actor *healthPotion = new Actor(x, y, TileCharacters::Default::PLUS, "medkit",
				TileColors::red);
		healthPotion->blocks = false;
		healthPotion->item = new Healer(4);
		return healthPotion;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateMedkit" << std::endl;
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
		case MapType::WOODS:
			monster = CreateMonsterForWoods(x, y, difficulty);
			break;
		case MapType::ROAD:
		default:
			throw 0;
			break;
		}
		return monster;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateMonster" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateSewerMutant(int x, int y){
	try{
		Actor *mutant = new Actor(x, y, TileCharacters::Default::M_LOWERCASE, "sewer mutant", TileColors::desaturatedLime);
		mutant->destructible = new MonsterDestructible(25, 1, 0, "dead mutant");
		mutant->attacker = new Attacker(4);
		mutant->ai = new MonsterAi();
		return mutant;
	}
	catch(...){
		std::cerr << "An error occurred in CreateSewerMutant" << std::endl;
		throw 0;
	}
}



Actor *ActorFactory::CreateGoblin(int x, int y){
	try{
		Actor *goblin = new Actor(x, y, TileCharacters::Default::G_LOWERCASE, "goblin", TileColors::darkestGreen);
		goblin->destructible = new MonsterDestructible(12, 1, 2, "dead goblin");
		goblin->attacker = new Attacker(3);
		goblin->ai = new MonsterAi();
		return goblin;
	}
	catch(...){
		std::cerr << "An error occurred in CreateGoblin" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateDirewolf(int x, int y){
	try{
		Actor *direWolf = new Actor(x, y, TileCharacters::Default::W_LOWERCASE, "wolf", TileColors::grey);
		direWolf->destructible = new MonsterDestructible(7, 0, 1, "dead wolf");
		direWolf->attacker = new Attacker(2);
		direWolf->ai = new MonsterAi(40);
		return direWolf;
	}
	catch(...){
		std::cerr << "An error occurred in CreateDirewolf" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateDirebear(int x, int y){
	try{
		Actor *direBear = new Actor(x, y, TileCharacters::Default::B_UPPERCASE, "dire bear", TileColors::darkerBrown);
		direBear->destructible = new MonsterDestructible(80, 0, 6, "dead bear");
		direBear->attacker = new Attacker(15);
		direBear->ai = new MonsterAi(40);
		return direBear;
	}
	catch(...){
		std::cerr << "An error occurred in CreateDirebear" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateGiantInsect(int x, int y){
	try{
		Actor *giantInsect = new Actor(x, y, TileCharacters::Default::I_LOWERCASE, "giant insect", TileColors::violet);
		giantInsect->destructible = new MonsterDestructible(40, 4, 24, "dead insect");
		giantInsect->attacker = new Attacker(9);
		giantInsect->ai = new MonsterAi();
		return giantInsect;
	}
	catch(...){
		std::cerr << "An error occurred in CreateGiantInsect" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateMonsterForCity(int x, int y, EnemyDifficulty difficulty){
	try{
		TCODRandom rng(x+y, TCOD_RNG_CMWC);
		Actor *monster = nullptr;
		int randInt = rng.getInt(1, 100);
		std::map<EnemyDifficulty, int> spawnChances = GenerateSpawnChances(difficulty);


		if(randInt <= spawnChances[EnemyDifficulty::EASY]){
			if(randInt %2 == 0)
				monster = CreateGremlin(x, y);
			else
				monster = CreateZombie(x, y);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::MEDIUM]){
			if(randInt %2 == 0)
				monster = CreateGoblin(x, y);
			else
				monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::HARD]){
			if(randInt %2 == 0)
				monster = CreateSewerMutant(x, y);
			else
				monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::VERY_HARD]){
			monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::INSANE]){
			monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::NIGHTMARE]){
			monster = CreateVampire(x, y, difficulty);
		}
		else
			throw 0;


		return monster;
	}
	catch(...){
		std::cerr << "An error occurred in CreateMonsterForCity" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateMonsterForWoods(int x, int y, EnemyDifficulty difficulty){
	try{
		TCODRandom rng(x+y, TCOD_RNG_CMWC);
		Actor *monster = nullptr;
		int randInt = rng.getInt(1, 100);
		std::map<EnemyDifficulty, int> spawnChances = GenerateSpawnChances(difficulty);

		if(randInt <= spawnChances[EnemyDifficulty::EASY]){
			if(randInt %2 == 0)
				monster = CreateGremlin(x, y);
			else
				monster = CreateDirewolf(x, y);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::MEDIUM]){
			if(randInt %2 == 0)
				monster = CreateGoblin(x, y);
			else
				monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::HARD]){
			monster = CreateGiantInsect(x, y);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::VERY_HARD]){
			monster = CreateVampire(x, y, difficulty);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::INSANE]){
			monster = CreateDirebear(x, y);
		}
		else if(randInt <= spawnChances[EnemyDifficulty::NIGHTMARE]){
			monster = CreateVampire(x, y, difficulty);
		}
		else
			throw 0;

		return monster;
	}
	catch(...){
		std::cerr << "An error occured in CreateMonsterForWoods" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateItem(int x, int y, EnemyDifficulty difficulty){
	try{
		TCODRandom rng(x+y, TCOD_RNG_CMWC);
		int intRand = rng.getInt(1, 100);
		Actor *item;
		switch(difficulty){
			case EnemyDifficulty::EASY:
				if(intRand < 60)
					item = CreateMedkit(x, y);
				else{
					if(intRand %2 == 0)
						item = CreateKnife(x ,y);
					else
						item = CreateNightstick(x, y);
				}

				break;
			case EnemyDifficulty::MEDIUM:
				if(intRand < 50)
					item = CreateMedkit(x, y);
				else{
					if(intRand %2 == 0)
						item = CreateKris(x ,y);
					else
						item = CreateTireIron(x, y);
				}

				break;
			case EnemyDifficulty::HARD:
			case EnemyDifficulty::VERY_HARD:
			case EnemyDifficulty::NIGHTMARE:
			case EnemyDifficulty::INSANE:
				if(intRand < 50)
					item = CreateMedkit(x, y);
				else{
					if(intRand %2 == 0)
						item = CreateMachete(x ,y);
					else
						item = CreateBaseballBat(x, y);
				}
				break;
			default:
				throw 0;
				break;
		}
		return item;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateItem" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateNightstick(int x, int y){
	try{
		Actor *nightstick = new Actor(x, y, TileCharacters::Default::EXCLAMATION, "nightstick",
				TileColors::lighterGrey);
		nightstick->blocks = false;
		nightstick->item = new MeleeWeapon(9);
		return nightstick;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateNightstick" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateTireIron(int x, int y){
	try{
		Actor *tireIron = new Actor(x, y, TileCharacters::Default::EXCLAMATION, "tire iron",
				TileColors::grey);
		tireIron->blocks = false;
		tireIron->item = new MeleeWeapon(18);
		return tireIron;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateTireIron" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateKnife(int x, int y){
	try{
		Actor *knife = new Actor(x, y, TileCharacters::Default::FORWARD_SLASH, "knife",
				TileColors::lighterGrey);
		knife->blocks = false;
		knife->item = new MeleeWeapon(7);
		return knife;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateKnife" << std::endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateKris(int x, int y){
	try{
		Actor *kris = new Actor(x, y, TileCharacters::Default::FORWARD_SLASH, "kris",
				TileColors::grey);
		kris->blocks = false;
		kris->item = new MeleeWeapon(22);
		return kris;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateNightStick" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateBaseballBat(int x, int y){
	try{
		Actor *baseballBat = new Actor(x, y, TileCharacters::Default::EXCLAMATION, "tire iron",
				TileColors::darkGrey);
		baseballBat->blocks = false;
		baseballBat->item = new MeleeWeapon(36);
		return baseballBat;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateBaseballBat" << std::endl;
		throw 0;
	}
}

Actor *ActorFactory::CreateMachete(int x, int y){
	try{
		Actor *machete = new Actor(x, y, TileCharacters::Default::AT_SIGN, "machete",
				TileColors::white);
		machete->blocks = false;
		machete->item = new MeleeWeapon(38);
		return machete;
	}
	catch(...){
		std::cerr << "An error occurred in ActorFactory::CreateMachete" << std::endl;
		throw 0;
	}
}

std::map<ActorFactory::EnemyDifficulty, int>  ActorFactory::GenerateSpawnChances(EnemyDifficulty difficulty){
	try{
		std::map<EnemyDifficulty, int> chancesForSpawn;
		switch(difficulty){
			case EnemyDifficulty::EASY:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 100;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 0;
				chancesForSpawn[EnemyDifficulty::HARD] = 0;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 0;
				chancesForSpawn[EnemyDifficulty::INSANE] = 0;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 0;
			}
			break;
			case EnemyDifficulty::MEDIUM:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 25;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 100;
				chancesForSpawn[EnemyDifficulty::HARD] = 0;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 0;
				chancesForSpawn[EnemyDifficulty::INSANE] = 0;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 0;
			}
			break;
			case EnemyDifficulty::HARD:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 12;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 25;
				chancesForSpawn[EnemyDifficulty::HARD] = 100;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 0;
				chancesForSpawn[EnemyDifficulty::INSANE] = 0;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 0;
			}
			break;
			case EnemyDifficulty::VERY_HARD:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 6;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 12;
				chancesForSpawn[EnemyDifficulty::HARD] = 25;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 100;
				chancesForSpawn[EnemyDifficulty::INSANE] = 0;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 0;
			}
			break;
			case EnemyDifficulty::INSANE:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 3;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 6;
				chancesForSpawn[EnemyDifficulty::HARD] = 12;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 25;
				chancesForSpawn[EnemyDifficulty::INSANE] = 100;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 0;
			}
			break;
			case EnemyDifficulty::NIGHTMARE:
			{
				chancesForSpawn[EnemyDifficulty::EASY] = 1;
				chancesForSpawn[EnemyDifficulty::MEDIUM] = 3;
				chancesForSpawn[EnemyDifficulty::HARD] = 6;
				chancesForSpawn[EnemyDifficulty::VERY_HARD] = 12;
				chancesForSpawn[EnemyDifficulty::INSANE] = 25;
				chancesForSpawn[EnemyDifficulty::NIGHTMARE] = 100;
			}
			break;
		}
		return chancesForSpawn;
	}

	catch(...){
		std::cerr << "An error occurred in GenerateChances" << std::endl;
		throw 0;
	}
}
