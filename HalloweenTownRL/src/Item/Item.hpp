#ifndef HALLOWEENTOWN_PICKABLE_HPP
#define HALLOWEENTOWN_PICKABLE_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"

class Actor;

class Item : public Persistent {
public :
	enum ItemType {
		HEALER,
		MELEE_WEAPON
	};
	Item(ItemType type);

	bool Pick(Actor *owner, Actor *wearer);
	virtual bool Use(Actor *owner, Actor *wearer);
	static Item *Create(TCODZip &zip);
	ItemType getType();
private:
	ItemType type;

};
#endif
