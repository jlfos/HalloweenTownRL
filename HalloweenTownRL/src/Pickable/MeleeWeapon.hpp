/*
 * MeleeWeapon.hpp
 *
 *  Created on: Jul 1, 2015
 */

#ifndef MELEEWEAPON_HPP
#define MELEEWEAPON_HPP
#include "libtcod.hpp"
#include "../Persistent.hpp"
#include "Pickable.hpp"
class Actor;

class MeleeWeapon : public Pickable{

private:
	float amount;
public:
	MeleeWeapon(float amount);
	~MeleeWeapon();
	bool Use(Actor *owner, Actor *wearer);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};


#endif /* MELEEWEAPON_HPP */
