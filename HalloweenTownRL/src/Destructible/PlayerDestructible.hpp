#ifndef HALLOWEENTOWN_PLAYERDESTRUCTIBLE_HPP
#define HALLOWEENTOWN_PLAYERDESTRUCTIBLE_HPP
#include <string>
#include "libtcod.hpp"
#include "Destructible.hpp"
class Actor;

class PlayerDestructible : public Destructible {
public :
	PlayerDestructible(float maxHp, float defense, std::string corpseName);
	void Die(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
