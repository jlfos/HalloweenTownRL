#include "../main.hpp"
#include <iostream>

using namespace std;

Actor *ActorFactory::CreateHero(int x, int y){
	try{
		Actor *player = new Actor(x, y,'@',"player",TCODColor::white);
		player->destructible=new PlayerDestructible(60,2,"your cadaver");
		player->attacker=new Attacker(10);
		player->ai = new PlayerAi();
		player->container= new Container(26);
		return player;
	}
	catch(...){
		cerr << "An error occurred in ActorFactory::CreateHero" << endl;
		throw 0;
	}
}


Actor *ActorFactory::CreateOrc(int x, int y){
	try{
		Actor *orc = new Actor(x,y,'o',"orc",
			TCODColor::desaturatedGreen);
		orc->destructible = new MonsterDestructible(10,0,"dead orc");
		orc->attacker = new Attacker(3);
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
		Actor *troll = new Actor(x,y,'T',"troll",
			 TCODColor::darkBlue);
		troll->destructible = new MonsterDestructible(16,1,"troll carcass");
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
		Actor *healthPotion = new Actor(x, y, '!', "health potion",
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
