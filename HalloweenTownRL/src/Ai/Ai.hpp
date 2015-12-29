
#ifndef HALLOWEENTOWN_AI_HPP
#define HALLOWEENTOWN_AI_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"
class Actor;

class Ai : public Persistent {
protected:
	enum  AiType{
		MONSTER, PLAYER, DOOR
	};

public:
	Ai(AiType type);
	virtual void Update(Actor *owner) = 0;
	static Ai *Create(TCODZip &zip);
	AiType GetAiType();


private:
	AiType type;

};
#endif
