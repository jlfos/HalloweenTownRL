#include <iostream>
#include <string>
#include "libtcod.hpp"
#include "../Actor/Actor.hpp"
#include "BossDestructible.hpp"
#include "Destructible.hpp"
#include "../Engine.hpp"
#include "../LoggerWrapper.hpp"
#include "MonsterDestructible.hpp"
#include "PlayerDestructible.hpp"
#include "../Tile/TileCharacters.hpp"
#include "../Tile/TileColors.hpp"

Destructible::Destructible(float maxHp, float defense, int experienceReward, std::string corpseName, TCODColor corpseColor, int corpseCharacter):
	maxHp(maxHp), hp(maxHp), defense(defense), experienceReward(experienceReward), corpseName(corpseName), corpseColor(corpseColor), corpseCharacter(corpseCharacter){
}

Destructible::Destructible(float maxHp, float defense, int experienceReward, std::string corpseName, TCODColor corpseColor):
			maxHp(maxHp), hp(maxHp), defense(defense), experienceReward(experienceReward), corpseName(corpseName), corpseColor(corpseColor){
	corpseCharacter = generateCorpseCharacter();
}


Destructible::Destructible(float maxHp, float defense, int experienceReward, std::string corpseName):
	maxHp(maxHp), hp(maxHp), defense(defense), experienceReward(experienceReward), corpseName(corpseName){

	corpseCharacter = generateCorpseCharacter();
	corpseColor = TileColors::darkRed;
}

int Destructible::generateCorpseCharacter() {
	int character;
	TCODRandom rng(TCODSystem::getElapsedMilli());
	int randInt = rng.getInt(2, 4);
	if (randInt % 2 == 0)
		character = TileCharacters::Default::PERCENT;
	else if (randInt % 3 == 0)
		character = TileCharacters::Default::BURST;
	else
		character = TileCharacters::Default::SWIRL;

	return character;


}

Destructible *Destructible::Create(TCODZip &zip){
	try{
		DestructibleType type = (DestructibleType)zip.getInt();

		Destructible *destructible = nullptr;
		switch(type){
			case MONSTER: destructible = new MonsterDestructible(0,0, 0, NULL); break;
			case PLAYER: destructible = new PlayerDestructible(0,0, NULL); break;
			case BOSS: destructible = new BossDestructible(0, 0, 0, NULL); break;
			default : throw 0; break;
		}
		destructible->Load(zip);
		return destructible;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Destructible::Create");
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
		LoggerWrapper::Error("An error occurred in Destructible::TakeDamage");
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
		LoggerWrapper::Error("An error occurred in Destructible::Heal");
		throw 0;
	}
}

void Destructible::Die(Actor *owner){
	try{
		owner->ch=corpseCharacter;
		owner->col=corpseColor;
		owner->name=corpseName;
		owner->blocks=false;
		engine.SendToBack(owner);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Destructible::Die");
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
		LoggerWrapper::Error("An error occurred in Destructible::Load");
		throw 0;
	}
}

void Destructible::Save(TCODZip &zip){
	try{
		zip.putFloat(maxHp);
		zip.putFloat(hp);
		zip.putFloat(defense);
		zip.putString(corpseName.c_str());
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Destructible::Save");
		throw 0;
	}
}
