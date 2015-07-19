/*
 * MonsterEntry.cpp
 *
 *  Created on: Jun 30, 2015
 */
#include <string>
#include "libtcod.hpp"

#include "MonsterEntry.hpp"
MonsterEntry::MonsterEntry(std::string name, std::string description, std::string initialYear, int characterCode, TCODColor characterColor):
	name(name), description(description), initialYear(initialYear), characterCode(characterCode), characterColor(characterColor), encountered(false){
}

bool MonsterEntry::Compare(const MonsterEntry &left, const MonsterEntry &right){
	if(left.name!=right.name)
		return true;
	else
		return false;
}



std::string MonsterEntry::getName(){
	return name;
}


std::string MonsterEntry::getDescription(){
	return description;
}

std::string MonsterEntry::getInitialYear(){
	return initialYear;
}

int MonsterEntry::getCharacterCode(){
	return characterCode;
}

TCODColor MonsterEntry::getCharacterColor(){
	return characterColor;
}
