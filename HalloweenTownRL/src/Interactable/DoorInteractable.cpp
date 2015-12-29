/*
 * DoorAi.cpp
 *
 *  Created on: Dec 28, 2015
 */
#include "../Actor/Actor.hpp"
#include "DoorInteractable.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "../LoggerWrapper.hpp"
#include "../Tile/TileColors.hpp"

const int CLOSED_CHARACTER = TileCharacters::Default::PLUS;
const int OPEN_CHARACTER = TileCharacters::Default::BACKSLASH;

//TODO The walkable/istransparent properties need to not only be set but change based off the state of the door. I don't think the current system even allows this.
DoorInteractable::DoorInteractable(Actor* owner) :Interactable(owner) {
}


Interactable::Result  DoorInteractable::Interact(Actor* instigator, LastAction action) {

	try {
		if(CanToggleState(instigator, action)){
				if(owner->ch == OPEN_CHARACTER){
					engine.gui->PushMessage(TileColors::greyLight,"You close the door.");
					owner->ch = CLOSED_CHARACTER;
					owner->blocks = true;
					return Result::SUCCESS;
				}
				else if(owner->ch == CLOSED_CHARACTER){
					engine.gui->PushMessage(TileColors::greyLight,"You open the door.");
					owner->ch = OPEN_CHARACTER;
					owner->blocks = false;
					return Result::SUCCESS;
				}
				else{
					LoggerWrapper::Error("An incorrect character '"+ std::to_string(owner->ch) +"' was used for a door");
					throw 0;
				}
		}
		else{
			return Result::NOTHING_HAPPENS;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in DoorInteractable::Interact");
		throw 0;
	}

}

bool DoorInteractable::CanToggleState(Actor* instigator, LastAction action) {
	if(action.virtualKey == TCOD_keycode_t::TCODK_DOWN && instigator->x == owner->x && instigator->y + 1 == owner->y){ //Instigator is north of the door and moves into it
		if(owner->ch == CLOSED_CHARACTER){
			return true;
		}
		else{
			return false;
		}
	}
	else if(action.virtualKey == TCOD_keycode_t::TCODK_UP && instigator->x == owner->x && instigator->y - 1== owner->y){ //Instigator is north of the door and moves into it
		if(owner->ch == CLOSED_CHARACTER){
			return true;
		}
		else{
			return false;
		}
	}
	else if(action.virtualKey == TCOD_keycode_t::TCODK_RIGHT && instigator->x + 1== owner->x && instigator->y == owner->y){ //Instigator is north of the door and moves into it
		if(owner->ch == CLOSED_CHARACTER){
			return true;
		}
		else{
			return false;
		}
	}
	else if(action.virtualKey == TCOD_keycode_t::TCODK_LEFT && instigator->x - 1== owner->x && instigator->y == owner->y){ //Instigator is north of the door and moves into it
		if(owner->ch == CLOSED_CHARACTER){
			return true;
		}
		else{
			return false;
		}
	}
	else if(action.virtualKey == TCOD_keycode_t::TCODK_CHAR && action.key == 'o' && instigator->x >= owner->x - 1 && instigator->x <= owner->x + 1 &&
			instigator->y >= owner->y - 1 && instigator->y <= owner->y + 1){
		return true;
	}
	else
		return false;
}


