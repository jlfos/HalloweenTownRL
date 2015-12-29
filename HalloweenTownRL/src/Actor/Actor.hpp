#ifndef HALLOWEENTOWN_ACTOR_HPP
#define HALLOWEENTOWN_ACTOR_HPP
#include <string>
#include <memory>
#include "libtcod.hpp"
#include "../Interactable/Interactable.hpp"
#include "../Persistent.hpp"
#include "../Lightsource.hpp"


class Attacker;
class Ai;
class Destructible;
class Container;
class Item;
class Actor: public Persistent {
public:


	int x, y; // position on map
	int ch; // ascii code
	TCODColor col; // color
	std::string name; // the actor's name
	bool blocks; // can we walk on this actor?
	Attacker* attacker; // something that deals damages
	Destructible* destructible; // something that can be damaged
	Ai* ai; // something self-updating
	Item* item; //something that can be picked up
	Container* container; //something that contains
	Lightsource*  lightsource;//something that emits light
	Interactable* interactable;//something that cn be interacted with
	Actor(int x, int y, int ch, std::string name, const TCODColor &col);
	~Actor();
	void Update();
	void Render() const;
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
