/*
 * BossDestructible.hpp
 *
 *  Created on: Jul 2, 2015
 */

#ifndef BOSSDESTRUCTIBLE_HPP
#define BOSSDESTRUCTIBLE_HPP
#include <string>
#include "Destructible.hpp"

class Actor;

class BossDestructible : public Destructible {

public:
	BossDestructible(float maxHp, float defense, int exp, std::string corpseName);
	void Die(Actor* owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};



#endif /* BOSSDESTRUCTIBLE_HPP */
