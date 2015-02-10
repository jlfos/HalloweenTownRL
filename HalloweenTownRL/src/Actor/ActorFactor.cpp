#include "../main.hpp"


Actor *ActorFactory::CreateHero(){
	Actor *player = new Actor(40,25,'@',"player",TCODColor::white);
    player->destructible=new PlayerDestructible(30,2,"your cadaver");
    player->attacker=new Attacker(5);
    player->ai = new PlayerAi();
    player->container= new Container(26);
	return player;
}


Actor *ActorFactory::CreateOrc(int x, int y){
    Actor *orc = new Actor(x,y,'o',"orc",
        TCODColor::desaturatedGreen);
    orc->destructible = new MonsterDestructible(10,0,"dead orc");
    orc->attacker = new Attacker(3);
    orc->ai = new MonsterAi();
    return orc;
}

Actor *ActorFactory::CreateTroll(int x, int y){
    Actor *troll = new Actor(x,y,'T',"troll",
         TCODColor::darkBlue);
    troll->destructible = new MonsterDestructible(16,1,"troll carcass");
    troll->attacker = new Attacker(4);
    troll->ai = new MonsterAi();
   return troll;

}

Actor *ActorFactory::CreatePotion(int x, int y){
	Actor *healthPotion = new Actor(x, y, '!', "health potion",
			TCODColor::violet);
	healthPotion->blocks = false;
	healthPotion->pickable = new Healer(4);
	return healthPotion;
}
