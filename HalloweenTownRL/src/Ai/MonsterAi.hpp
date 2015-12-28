
#ifndef HALLOWEENTOWN_MONSTERAI_HPP
#define HALLOWEENTOWN_MONSTERAI_HPP

#include "libtcod.hpp"
#include "Ai.hpp"

class Actor;
class MonsterAi : public Ai {
public:
	MonsterAi();
	MonsterAi(int trackingDistance);
	void Update(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
	void Interact(Actor *owner, Actor* target);
protected:
	void MoveOrAttack(Actor *owner, int targetX, int targetY);
	int moveCount;
	int trackingDistance;
};
#endif
