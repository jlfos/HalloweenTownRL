#ifndef HALLOWEENTOWN_MONSTERDESTRUCTIBLE_HPP
#define HALLOWEENTOWN_MONSTERDESTRUCTIBLE
#include <string>
#include "libtcod.hpp"

class Actor;

class MonsterDestructible : public Destructible {
public :
	MonsterDestructible(float maxHp, float defense, int exp, std::string corpseName, TCODColor corpseColor, int corpseCharacter);
	MonsterDestructible(float maxHp, float defense, int exp, std::string corpseName, TCODColor corpseColor);
	MonsterDestructible(float maxHp, float defense, int exp, std::string corpseName);
	void Die(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
