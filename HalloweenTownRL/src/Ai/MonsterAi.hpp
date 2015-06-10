
#ifndef HALLOWEENTOWN_MONSTERAI_HPP
#define HALLOWEENTOWN_MONSTERAI_HPP

#include "libtcod.hpp"
#include "Ai.hpp"

class Actor;
class MonsterAi : public Ai {
public:
	MonsterAi();
	void Update(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
protected:
	void MoveOrAttack(Actor *owner, int targetX, int targetY);
	int moveCount;
};
#endif
