
#ifndef HALLOWEENTOWN_AI_HPP
#define HALLOWEENTOWN_AI_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"
class Actor;

class Ai : public Persistent {
public:
	virtual void Update(Actor *owner)=0;
	static Ai *Create(TCODZip &zip);
protected:
	enum AiType{
		MONSTER, PLAYER
	};
};
#endif
