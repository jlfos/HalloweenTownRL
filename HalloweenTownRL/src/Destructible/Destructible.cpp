#include <iostream>
#include "libtcod.hpp"
#include "../main.hpp"


Destructible::Destructible(float maxHp, float defense, int experienceReward, const char *corpseName) :
	maxHp(maxHp), hp(maxHp), defense(defense), experienceReward(experienceReward), corpseName(corpseName){
}


Destructible *Destructible::Create(TCODZip &zip){
	try{
		DestructibleType type = (DestructibleType)zip.getInt();

		Destructible *destructible = nullptr;
		switch(type){
			case MONSTER: destructible = new MonsterDestructible(0,0, 0, NULL); break;
			case PLAYER: destructible = new PlayerDestructible(0,0, NULL); break;
			default : break;
		}
		destructible->Load(zip);
		return destructible;
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::Create" << std::endl;
		throw 0;
	}
}


void Destructible::SetDefense(float defense){
	this->defense = defense;
}

float Destructible::GetDefense(){
	return defense;
}

float Destructible::TakeDamage(Actor *owner, float damage){
	try{
		damage -= defense;
		if( damage>0){
			hp -= damage;
			if (hp <=0){
				Die(owner);
			}
		}
		else{
			damage = 0;
		}
		return damage;
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::TakeDamage" << std::endl;
		throw 0;
	}
}

float Destructible::Heal(float amount){
	try{
		hp += amount;

		if(hp > maxHp){
			amount -= hp - maxHp;
			hp = maxHp;
		}
		return amount;
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::Heal" << std::endl;
		throw 0;
	}
}

void Destructible::Die(Actor *owner){
	try{
		owner->ch='%';
		owner->col=TCODColor::darkRed;
		owner->name=corpseName;
		owner->blocks=false;
		engine.SendToBack(owner);
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::Die" << std::endl;
		throw 0;
	}
}

int Destructible::ExperienceReward(){
	return experienceReward;
}

void Destructible::IncreaseTotalHealth(float health){
	maxHp += health;
	hp += health;
}

void Destructible::Load(TCODZip &zip){
	try{
		maxHp=zip.getFloat();
		hp=zip.getFloat();
		defense=zip.getFloat();
		corpseName=strdup(zip.getString());
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::Load" << std::endl;
		throw 0;
	}
}

void Destructible::Save(TCODZip &zip){
	try{
		zip.putFloat(maxHp);
		zip.putFloat(hp);
		zip.putFloat(defense);
		zip.putString(corpseName);
	}
	catch(...){
		std::cerr << "An error occurred in Destructible::Save" << std::endl;
		throw 0;
	}
}
