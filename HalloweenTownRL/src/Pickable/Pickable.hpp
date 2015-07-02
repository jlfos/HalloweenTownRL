#ifndef HALLOWEENTOWN_PICKABLE_HPP
#define HALLOWEENTOWN_PICKABLE_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"

class Actor;

//TODO Change to item
class Pickable : public Persistent {
public :
	enum PickableType {
		HEALER,
		MELEE_WEAPON
	};
	bool Pick(Actor *owner, Actor *wearer);
	virtual bool Use(Actor *owner, Actor *wearer);
	static Pickable *Create(TCODZip &zip);
	PickableType getType();
private:
	PickableType type;

};
#endif
