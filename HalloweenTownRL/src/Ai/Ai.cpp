#include <iostream>
#include "libtcod.hpp"
#include "Ai.hpp"
#include "PlayerAi.hpp"
#include "../LoggerWrapper.hpp"
#include "MonsterAi.hpp"

Ai *Ai::Create(TCODZip &zip){
	try{
		AiType type = (AiType)zip.getInt();
		Ai *ai = nullptr;
		switch(type){
			case PLAYER : ai = new PlayerAi(); break;
			case MONSTER: ai = new MonsterAi(); break;
		}
		ai->Load(zip);
		return ai;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Ai::create");
		throw 0;
	}
}

