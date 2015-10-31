#ifndef HALLOWEENTOWN_ACTOR_HPP
#define HALLOWEENTOWN_ACTOR_HPP
#include <string>
#include <memory>
#include "libtcod.hpp"
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
	std::shared_ptr<Attacker> attacker; // something that deals damages
	std::shared_ptr<Destructible> destructible; // something that can be damaged
	std::shared_ptr<Ai> ai; // something self-updating
	std::shared_ptr<Item> item; //something that can be picked up
	std::shared_ptr<Container> container; //something that contains
	std::shared_ptr<Lightsource>  lightsource;//something that emits light
	Actor(int x, int y, int ch, std::string name, const TCODColor &col);
	~Actor();
	void Update();
	void Render() const;
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
