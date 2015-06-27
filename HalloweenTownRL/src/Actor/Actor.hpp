#ifndef HALLOWEENTOWN_ACTOR_HPP
#define HALLOWEENTOWN_ACTOR_HPP

#include "libtcod.hpp"
#include "../Persistent.hpp"



class Attacker;
class Ai;
class Destructible;
class Container;
class Pickable;
class Actor: public Persistent {
public:


	int x, y; // position on map
	int ch; // ascii code
	TCODColor col; // color
	const char *name; // the actor's name
	bool blocks; // can we walk on this actor?
	Attacker *attacker; // something that deals damages
	Destructible *destructible; // something that can be damaged
	Ai *ai; // something self-updating
	Pickable *pickable; //something that can be picked up
	Container *container; //something that contains

	Actor(int x, int y, int ch, const char *name, const TCODColor &col);
	~Actor();
	void Update();
	void Render() const;
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
