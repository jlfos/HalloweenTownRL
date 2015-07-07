#ifndef HALLOWEENTOWN_HEALER_HPP
#define HALLOWEENTOWN_HEALER_HPP

#include "libtcod.hpp"
#include "Item.hpp"

class Actor;

class Healer : public Item {
public :
	float amount; // how many hp

	Healer(float amount);
	~Healer();
	bool Use(Actor *owner, Actor *wearer);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
