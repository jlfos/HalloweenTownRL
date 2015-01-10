#include <stdio.h>
#include "../main.hpp"



Ai *Ai::create(TCODZip &zip){
	AiType type = (AiType)zip.getInt();
	Ai *ai = nullptr;
	switch(type){
		case PLAYER : ai = new PlayerAi(); break;
		case MONSTER: ai = new MonsterAi(); break;
	}
	ai->load(zip);
	return ai;
}

