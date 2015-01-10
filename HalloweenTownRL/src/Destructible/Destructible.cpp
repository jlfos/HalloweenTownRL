#include <stdio.h>
#include <stdlib.h>
#include "libtcod.hpp"
#include "../main.hpp"


Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
	maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName){
}


Destructible *Destructible::create(TCODZip &zip){
	DestructibleType type = (DestructibleType)zip.getInt();
	Destructible *destructible = NULL;
	switch(type){
		case MONSTER: destructible = new MonsterDestructible(0,0,NULL); break;
		case PLAYER: destructible = new PlayerDestructible(0,0, NULL); break;
		default : break;
	}
	destructible->load(zip);
	return destructible;
}


float Destructible::takeDamage(Actor *owner, float damage){
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

float Destructible::heal(float amount){
	hp += amount;
	if(hp > maxHp){
		amount -= hp - maxHp;
		hp = maxHp;
	}
	return amount;
}

void Destructible::die(Actor *owner){
	owner->ch='%';
	owner->col=TCODColor::darkRed;
	owner->name=corpseName;
	owner->blocks=false;
	engine.sendToBack(owner);

}

void Destructible::load(TCODZip &zip){
	maxHp=zip.getFloat();
	hp=zip.getFloat();
	defense=zip.getFloat();
	corpseName=strdup(zip.getString());
}

void Destructible::save(TCODZip &zip){
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(defense);
	zip.putString(corpseName);
}
