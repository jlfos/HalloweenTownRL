/*
 * MonsterEntry.hpp
 *
 *  Created on: Jun 30, 2015
 */

#ifndef MONSTERENTRY_HPP
#define MONSTERENTRY_HPP
#include <string>
#include "libtcod.hpp"


class MonsterEntry {
public:
	MonsterEntry(std::string name, std::string description, std::string initialYear, int characterCode, TCODColor characterColor);
	static bool Compare(const MonsterEntry &left, const MonsterEntry&right);
	std::string getName();
	std::string getDescription();
	std::string getInitialYear();
	int getCharacterCode();
	TCODColor getCharacterColor();

private:
	std::string name;
	std::string description;
	std::string initialYear;
	int characterCode;
	TCODColor characterColor;
	bool encountered;
};



#endif /* MONSTERENTRY_HPP */
