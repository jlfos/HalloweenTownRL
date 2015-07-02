#ifndef HALLOWEENTOWN_ATTACKER_HPP
#define HALLOWEENTOWN_ATTACKER_HPP
#include <string>
#include "libtcod.hpp"
#include "Persistent.hpp"

class Actor;

class Attacker : public Persistent {
public :
	void setBasePower(float basePower);
	void setModifierPower(float modifierPower);
	float getBasePower();
	float getAttackPower();
	float getModifierPower();
	void setWeapon(std::string string);
	std::string getWeapon();
	Attacker(float power);
	Attacker(float power, std::string weapon);
	void Attack(Actor *owner, Actor *target);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
private:
	float basePower;
	float modifierPower;
	std::string weapon;
};
#endif
