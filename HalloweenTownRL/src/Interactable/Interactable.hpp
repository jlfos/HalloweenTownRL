/*
 * Interactable.hpp
 *
 *  Created on: Dec 28, 2015
 */

#ifndef INTERACTABLE_HPP_
#define INTERACTABLE_HPP_
#include "libtcod.hpp"

class Actor;

struct LastAction{
	TCOD_keycode_t virtualKey;
	char key;
};

class Interactable {

public:
	enum class Result {SUCCESS, FAILURE, NOTHING_HAPPENS};
	Interactable(Actor *owner);

	virtual Result Interact(Actor* instigator, LastAction action) = 0;

protected:
	Actor* owner;

};




#endif /* INTERACTABLE_HPP_ */
