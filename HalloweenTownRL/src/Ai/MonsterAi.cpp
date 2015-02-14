#include <iostream>
#include <math.h>
#include "../main.hpp"

using namespace std;

static const int TRACKING_TURNS=3;

MonsterAi::MonsterAi() : moveCount(0) {
}

void MonsterAi::update(Actor *owner){
	try{
		if ( owner->destructible && owner->destructible->isDead() ) {
			return;
		}
		if ( engine.map->isInFov(owner->x,owner->y) ) {
			// we can see the player. move towards him
			moveCount=TRACKING_TURNS;
		}
		else {
			moveCount--;
		}
	   if ( moveCount > 0 ) {
		moveOrAttack(owner, engine.player->x,engine.player->y);
	   }
	}
	catch(...){
		cerr << "An error occurred in MonsterAi::update" << endl;
		throw 0;
	}
}

void MonsterAi::moveOrAttack(Actor *owner, int targetX, int targetY){
	try{
		int dx = targetX - owner->x;
		int dy = targetY - owner->y;
		int stepdx = (dx > 0 ? 1:-1);
		int stepdy = (dy > 0 ? 1:-1);
		float distance = sqrtf(dx*dx+dy*dy);
		if(distance >=2){
			dx = (int)(round(dx/distance));
			dy = (int)(round(dy/distance));
			if(engine.map->canWalk(owner->x+dx, owner->y+dy)){
				owner->x += dx;
				owner->y += dy;
			}
			else if ( engine.map->canWalk(owner->x+stepdx,owner->y) ) {
				owner->x += stepdx;
			}
			else if ( engine.map->canWalk(owner->x,owner->y+stepdy) ) {
				owner->y += stepdy;
			}
		}
		else if ( owner->attacker ) {
				owner->attacker->attack(owner,engine.player);
		}
	}
	catch(...){
		cerr << "An error occurred in MonsterAi::moveOrAttack" << endl;
		throw 0;
	}
}

void MonsterAi::load(TCODZip &zip){
	try{
		moveCount=zip.getInt();
	}
	catch(...){
		cerr << "An error occurred in MonsterAi::load" << endl;
		throw 0;
	}
}

void MonsterAi::save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		zip.putInt(moveCount);
	}
	catch(...){
		cerr << "An error occurred in MonsterAi::save" << endl;
		throw 0;
	}
}
