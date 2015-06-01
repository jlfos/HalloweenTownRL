#include <iostream>
#include <math.h>
#include "../main.hpp"

static const int TRACKING_TURNS=3;

MonsterAi::MonsterAi() : moveCount(0) {
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
		std::cerr << "An error occurred in MonsterAi::update" << std::endl;
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
		if(distance >=2){
			if(distance >= 20){
				dx = rng.getInt(-1, 1);
				dy = rng.getInt(-1, 1);
			}
			else{
				dx = (int)(round(dx/distance));
				dy = (int)(round(dy/distance));
			}

			if(engine.currentMap->CanWalk(owner->x+dx, owner->y+dy)){
				owner->x += dx;
				owner->y += dy;
			}
			else if ( engine.currentMap->CanWalk(owner->x+stepdx,owner->y) ) {
				owner->x += stepdx;
			}
			else if ( engine.currentMap->CanWalk(owner->x,owner->y+stepdy) ) {
				owner->y += stepdy;
			}
		}
		else if ( owner->attacker ) {
				owner->attacker->Attack(owner,engine.player);
		}
	}
	catch(...){
		std::cerr << "An error occurred in MonsterAi::moveOrAttack" << std::endl;
		throw 0;
	}
}

void MonsterAi::Load(TCODZip &zip){
	try{
		moveCount=zip.getInt();
	}
	catch(...){
		std::cerr << "An error occurred in MonsterAi::load" << std::endl;
		throw 0;
	}
}

void MonsterAi::Save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		zip.putInt(moveCount);
	}
	catch(...){
		std::cerr << "An error occurred in MonsterAi::save" << std::endl;
		throw 0;
	}
}
