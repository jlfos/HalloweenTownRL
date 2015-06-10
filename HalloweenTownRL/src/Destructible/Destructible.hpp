#ifndef HALLOWEENTOWN_DESTRUCTIBLE_HPP
#define HALLOWEENTOWN_DESTRUCTIBLE_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"

class Actor;

class Destructible : public Persistent {
public :
	Destructible(float maxHp, float defense, int exp, const char *corpseName);
	inline bool IsDead() { return hp <= 0; }
	float TakeDamage(Actor *owner, float damage);
	float Heal(float amount);
	virtual void Die(Actor *owner);
	int ExperienceReward();
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
	static Destructible *Create(TCODZip &zip);
	void IncreaseTotalHealth(float health);
	void SetDefense(float defense);
	float GetDefense();
	float maxHp; // maximum health points
	float hp; // current health points
protected:
	enum DestructibleType{
		MONSTER, PLAYER
	};
private:
	float defense; // hit points deflected
	int experienceReward;
	const char *corpseName; // the actor's name once dead/destroyed
};
#endif
