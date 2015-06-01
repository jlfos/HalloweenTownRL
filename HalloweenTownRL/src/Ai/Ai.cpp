#include <iostream>
#include "../main.hpp"


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
		std::cerr << "An error occurred in Ai::create" << std::endl;
		throw 0;
	}
}

