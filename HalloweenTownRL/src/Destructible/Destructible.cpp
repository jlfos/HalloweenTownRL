#include <iostream>
#include "libtcod.hpp"
#include "../main.hpp"

using namespace std;

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
	maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName){
}


Destructible *Destructible::create(TCODZip &zip){
	try{
		DestructibleType type = (DestructibleType)zip.getInt();

		Destructible *destructible = nullptr;
		switch(type){
			case MONSTER: destructible = new MonsterDestructible(0,0,NULL); break;
			case PLAYER: destructible = new PlayerDestructible(0,0, NULL); break;
			default : break;
		}
		destructible->load(zip);
		return destructible;
	}
	catch(...){
		cerr << "An error occurred in Destructible::create" << endl;
		throw 0;
	}
}


float Destructible::takeDamage(Actor *owner, float damage){
	try{
		damage -= defense;
		if( damage>0){
			hp -= damage;
			if (hp <=0){
				die(owner);
			}
		}
		else{
			damage = 0;
		}
		return damage;
	}
	catch(...){
		cerr << "An error occurred in Destructible::takeDamage" << endl;
		throw 0;
	}
}

float Destructible::heal(float amount){
	try{
		hp += amount;

		if(hp > maxHp){
			amount -= hp - maxHp;
			hp = maxHp;
		}
		return amount;
	}
	catch(...){
		cerr << "An error occurred in Destructible::heal" << endl;
		throw 0;
	}
}

void Destructible::die(Actor *owner){
	try{
		owner->ch='%';
		owner->col=TCODColor::darkRed;
		owner->name=corpseName;
		owner->blocks=false;
		engine.sendToBack(owner);
	}
	catch(...){
		cerr << "An error occurred in Destructible::die" << endl;
		throw 0;
	}
}

void Destructible::load(TCODZip &zip){
	try{
		maxHp=zip.getFloat();
		hp=zip.getFloat();
		defense=zip.getFloat();
		corpseName=strdup(zip.getString());
	}
	catch(...){
		cerr << "An error occurred in Destructible::load" << endl;
		throw 0;
	}
}

void Destructible::save(TCODZip &zip){
	try{
		zip.putFloat(maxHp);
		zip.putFloat(hp);
		zip.putFloat(defense);
		zip.putString(corpseName);
	}
	catch(...){
		cerr << "An error occurred in Destructible::save" << endl;
		throw 0;
	}
}
