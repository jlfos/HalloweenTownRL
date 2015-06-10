#ifndef HALLOWEENTOWN_ATTACKER_HPP
#define HALLOWEENTOWN_ATTACKER_HPP

#include "libtcod.hpp"
#include "Persistent.hpp"

class Actor;

class Attacker : public Persistent {
public :
	void SetPower(float power);
	float GetPower();
	Attacker(float power);
	void Attack(Actor *owner, Actor *target);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
private:
	float power;
};
#endif
