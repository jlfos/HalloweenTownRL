#include <iostream>
#include <math.h>

#include "../Actor/Actor.hpp"
#include "../Attacker.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../LoggerWrapper.hpp"
#include "MonsterAi.hpp"



static const int TRACKING_TURNS=3;

MonsterAi::MonsterAi(int trackingDistance) : moveCount(0), trackingDistance(trackingDistance) {

}

MonsterAi::MonsterAi() : MonsterAi(20)  {
}

void MonsterAi::Update(Actor *owner){
	try{
		if ( owner->destructible && owner->destructible->IsDead() ) {
			return;
		}
		if ( engine.currentMap->IsInFov(owner->x,owner->y) ) {
			// we can see the player. move towards him
			moveCount=TRACKING_TURNS;
		}
		else {
			moveCount--;
		}
	   if ( moveCount > 0 ) {
		MoveOrAttack(owner, engine.player->x,engine.player->y);
	   }
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterAi::update");
		throw 0;
	}
}

void MonsterAi::MoveOrAttack(Actor *owner, int targetX, int targetY){
	try{
		TCODRandom rng;
		int dx = targetX - owner->x;
		int dy = targetY - owner->y;
		int stepdx = (dx > 0 ? 1:-1);
		int stepdy = (dy > 0 ? 1:-1);
		float distance = sqrtf(dx*dx+dy*dy);
		if(distance >=2){ //not within striking distance
			if(distance >= 20){ //too far for monster to track
				dx = rng.getInt(-1, 1);
				dy = rng.getInt(-1, 1);
			}
			else{ //monster is tracking
				dx = (int)(round(dx/distance));
				dy = (int)(round(dy/distance));
			}

			if(engine.currentMap->CanWalk(owner->x+dx, owner->y+dy)){ //Can you move directly towards target?
				owner->x += dx;
				owner->y += dy;
			}
			else if ( engine.currentMap->CanWalk(owner->x+stepdx,owner->y) ) {  //Can you move horizontally to target?
				owner->x += stepdx;
			}
			else if ( engine.currentMap->CanWalk(owner->x,owner->y+stepdy) ) { //Can you move vertically to target?
				owner->y += stepdy;
			}
		}
		else if ( owner->attacker ) { //within striking distance
				owner->attacker->Attack(owner,engine.player);
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterAi::moveOrAttack");
		throw 0;
	}
}

void MonsterAi::Load(TCODZip &zip){
	try{
		moveCount=zip.getInt();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterAi::load");
		throw 0;
	}
}

void MonsterAi::Save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		zip.putInt(moveCount);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterAi::save");
		throw 0;
	}
}
