#ifndef HALLOWEENTOWN_DESTRUCTIBLE_HPP
#define HALLOWEENTOWN_DESTRUCTIBLE_HPP
#include <string>
#include "libtcod.hpp"
#include "../Persistent.hpp"

class Actor;

class Destructible : public Persistent {
public :
	Destructible(float maxHp, float defense, int exp, std::string corpseName, TCODColor corpseColor, int corpseCharacter);
	Destructible(float maxHp, float defense, int exp, std::string corpseName, TCODColor corpseColor);
	Destructible(float maxHp, float defense, int exp, std::string corpseName);
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
	std::string corpseName; // the actor's name once dead/destroyed
	TCODColor corpseColor; // the actor's color once dead/destroyed
	int corpseCharacter; // the actor's character once dead/destroyed
};
#endif
