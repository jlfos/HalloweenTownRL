/*
 * DoorAi.hpp
 *
 *  Created on: Dec 28, 2015
 */

#ifndef DOOR_INTERACTABLE_HPP
#define DOOR_INTERACTABLE_HPP

#include "Interactable.hpp"
#include "../Tile/TileCharacters.hpp"

class DoorInteractable : public Interactable {

public:
	DoorInteractable(Actor *owner);

	Interactable::Result Interact(Actor* instigator, LastAction action);

private:
	bool CanToggleState(Actor* instigator, LastAction action);

};



#endif /* DOOR_INTERACTABLE_HPP */
