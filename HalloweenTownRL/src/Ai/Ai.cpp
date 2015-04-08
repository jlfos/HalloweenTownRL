#include <iostream>
#include "../main.hpp"

using namespace std;

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
		cerr << "An error occurred in Ai::create" << endl;
		throw 0;
	}
}

