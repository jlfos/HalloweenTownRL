#include <iostream>
#include <chrono>
#include <thread>
#include "../Actor/Actor.hpp"
#include "../Interactable/DoorInteractable.hpp"
#include "../UI/Gui.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/HelpScreen.hpp"
#include "../Interactable/Interactable.hpp"
#include "../UI/LevelUpMenu.hpp"
#include "../LoggerWrapper.hpp"
#include "../UI/MonsterLogMenu.hpp"
#include "../Item/Item.hpp"
#include "../Tile/TileColors.hpp"
#include "PlayerAi.hpp"

#include "../UI/Console/InventoryConsole.hpp"//TODO remove

const int LEVEL_UP_BASE=1;
const int LEVEL_UP_INCREASE=20;

PlayerAi::PlayerAi() : Ai(Ai::AiType::PLAYER), experienceLevel(1), currentExperience(0), currentLevelGoal(LEVEL_UP_BASE) {

}

void PlayerAi::Update(Actor *owner){
	try{
		bool isDead = false;
		if(owner->destructible && owner->destructible->IsDead()){
			isDead = true;
		}
		if(!isDead){
			switch(engine.lastKey.vk) {
			case TCODK_UP : MoveUp(owner); break;
			case TCODK_DOWN : MoveDown(owner); break;
			case TCODK_LEFT : MoveLeft(owner); break;
			case TCODK_RIGHT : MoveRight(owner); break;
			case TCODK_ESCAPE :LoadMenu(); break;
			case TCODK_SPACE : std::cout <<"X: " << owner->x <<" Y: " << owner->y<< std::endl; break;
			case TCODK_CHAR : HandleActionKey(owner, engine.lastKey.c); break;
				default:break;
			}
		}
		else{
			switch(engine.lastKey.vk) {
			case TCODK_ESCAPE :
				LoadMenu();
			break;
				default:break;
			}
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::Update");
		throw 0;
	}

}


void PlayerAi::MoveLeft(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (MoveOrAttack(owner, owner->x-1,owner->y)) {
			engine.currentMap->ComputeFov();
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::MoveLeft");
		throw 0;
	}
}

void PlayerAi::MoveDown(Actor *owner){
    try{
		engine.gameStatus=Engine::NEW_TURN;
		if (MoveOrAttack(owner, owner->x,owner->y+1)) {
			engine.currentMap->ComputeFov();
		}
    }
    catch(...){
    	LoggerWrapper::Error("An error occurred in PlayerAi::MoveDown");
		throw 0;
    }
}


void PlayerAi::MoveRight(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (MoveOrAttack(owner, owner->x+1,owner->y)) {
			engine.currentMap->ComputeFov();
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::MoveRight");
		throw 0;
	}
}

void PlayerAi::MoveUp(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (MoveOrAttack(owner, owner->x, owner->y-1)) {
			engine.currentMap->ComputeFov();
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::MoveUp");
		throw 0;
	}
}

void PlayerAi::LoadMenu(){
	try{
		//engine.save();
		engine.Load();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::LoadMenu");
		throw 0;
	}
}

Interactable::Result PlayerAi::Interact(Actor *owner, Actor* target){
	try {
		if(owner && target){

			if(target->interactable){ //Interact
				LastAction action;
				action.virtualKey = engine.lastKey.vk;
				action.key = engine.lastKey.c;
				return target->interactable->Interact(owner, action);
			}
			else if(target->destructible && !target->destructible->IsDead()) { //Attack
				owner->attacker->Attack(owner, target);
				if(target->destructible->IsDead()){
					currentExperience += target->destructible->ExperienceReward();
					if(LevelUpOccurred()){
						LevelUpPlayer(owner);
					}
				}
				return Interactable::Result::SUCCESS;
			}
			else{
				if(target->blocks){
					return Interactable::Result::FAILURE;
				}
				else
					return Interactable::Result::NOTHING_HAPPENS;
			}
		}
		else{
			LoggerWrapper::Error("Null pointers are not allowed");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in PlayerAi::Interact");
		throw 0;
	}

}

bool PlayerAi::MoveOrAttack(Actor *owner, int targetX, int targetY){
	try{
		Map::TileType type = engine.currentMap->GetTileType(targetX, targetY);
		if(type == Map::TileType::WALL)  return false;
		else if(type == Map::TileType::TOP_EDGE ||
				type == Map::TileType::RIGHT_EDGE ||
				type == Map::TileType::BOTTOM_EDGE ||
				type == Map::TileType::LEFT_EDGE) {
				engine.NextLevel(type);
				return false;
		}
		for(Actor* actor : engine.actors){
			if(actor->x == targetX && actor->y == targetY){
				Interactable::Result result = Interact(owner, actor);
				if(result == Interactable::Result::SUCCESS || result == Interactable::Result::FAILURE)
					return false;
			}
		}

		for(Actor* actor : engine.actors){
			bool corpseOrItem =(actor->destructible && actor->destructible->IsDead()) || actor->item;
			if(corpseOrItem && actor->x == targetX && actor->y == targetY){
				engine.gui->PushMessage(TileColors::greyLight,"There's a %s here", (actor->name).c_str() );
			}
		}
		engine.currentMap->ComputeLight(owner, false);
		owner->x=targetX;
		owner->y=targetY;
		return true;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::MoveOrAttack");
		throw 0;
	}
}

int PlayerAi::GetLevel(){
	return experienceLevel;
}

void PlayerAi::LevelUpPlayer(Actor* player){
	try{
		experienceLevel++;
		currentLevelGoal += LEVEL_UP_INCREASE;
		LevelUpMenu levelUpMenu;
		std::string result = levelUpMenu.Pick();
		engine.gui->PushMessage(TileColors::blue,"You leveled up! You are now level %d.", experienceLevel );
		if(result == "Strength")
			player->attacker->setBasePower(player->attacker->getBasePower()+3 );

		else if(result == "Agility")
			player->destructible->SetDefense(player->destructible->GetDefense()+1 );

		else if(result == "Constitution")
			player->destructible->IncreaseTotalHealth(10);
	}
	catch(...){
		LoggerWrapper::Error("An error occured in PlayerAi::LevelUpPlayer");
		throw 0;
	}

}

bool PlayerAi::LevelUpOccurred(){
	return currentExperience > currentLevelGoal;
}


void PlayerAi::ShowHelp() {
	try {
		HelpScreen help;
		help.Show();
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ShowHelp");
		throw 0;
	}
}

void PlayerAi::HandleActionKey(Actor *owner, int ascii) {
	try
	{
			switch(ascii) {

			case 'g' : // pickup item
			{
				bool found=false;
				for (Actor *actor : engine.actors) {
					if ( actor->item && actor->x == owner->x && actor->y == owner->y ) {
						if (actor->item->Pick(actor,owner)) {
							found=true;
							engine.gui->PushMessage(TileColors::greyLight,"You picked up the %s.",
								(actor->name).c_str());
							engine.currentMap->actors.remove(actor);
							break;
						} else if (! found) {
							found=true;
							engine.gui->PushMessage(TileColors::red,"Your inventory is full.");
						}
					}
				}
				if (!found) {
					engine.gui->PushMessage(TileColors::greyLight,"There's nothing here that you can pick up.");
				}
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'i' : // display inventory
			{
				InventoryConsole inventory;
			}
			break;
			case '.' : //wait
			{
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'l': //activate look mode
			{
				PlayerLook(owner);
			}
			break;
			case 'L': //view event log
			{
				viewLog();
			}
			break;
			case '?' : //view help screen
			{
				ShowHelp();
			}
			break;
			case 'm' :
			{
				MonsterLogMenu monsterLog;
				monsterLog.PopulateMenu(false);
				std::string result = monsterLog.Pick();

			}
			break;
			case 'o':
			{
				//TODO this does not work well for the case of having more then on door around you.
				for(Actor* actor : engine.actors){
					if(actor != owner && actor->interactable && actor->x >= owner->x - 1 && actor->x <= owner->x + 1 &&
							actor->y >= owner->y - 1 && actor->y <= owner->y + 1){
						LastAction action;
						action.key = 'o';
						action.virtualKey = TCODK_CHAR;
						actor->interactable->Interact(owner, action);
						return;
					}
				}
				engine.gui->PushMessage(TileColors::greyLight,"There are no doors around for you to open.");

			}
			break;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::handleActionKey");
		throw 0;
	}
}

void PlayerAi::PlayerLook(Actor* player){
	try{
		bool lookMode = true;
		int cursorX = player->x;
		int cursorY = player->y;
		unsigned int currentTime;
		bool cursorOn = true;
		unsigned int previousTime = TCODSystem::getElapsedMilli();
	//	int previousTime = ;
		while(lookMode){

			currentTime = TCODSystem::getElapsedMilli();
			if(currentTime >= previousTime + 230){

				if(cursorOn){
					TCODConsole::root->setChar(cursorX, cursorY, 219);
					TCODConsole::root->setCharForeground(cursorX, cursorY, TileColors::white);
					TCODConsole::flush();
					cursorOn = false;
				}
				else{
					engine.Render();
					cursorOn = true;
				}
				previousTime = currentTime;

			}

			TCOD_key_t lastKey;
			TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
			switch(lastKey.vk) {
			case TCODK_UP : cursorY--; engine.Render(); break;
			case TCODK_DOWN : cursorY++; engine.Render(); break;
			case TCODK_LEFT : cursorX--; engine.Render(); break;
			case TCODK_RIGHT : cursorX++; engine.Render(); break;
			case TCODK_ESCAPE :lookMode = false; engine.Render(); break;
			case TCODK_ENTER :
				for(Actor* actor : engine.actors){
					if(actor->x == cursorX && actor->y == cursorY){
						engine.gui->PushMessage(TileColors::grey, "A %s", actor->name.c_str());
					}
				}

				break;
				default:break;
			}


		}
	}
	catch(...){
		LoggerWrapper::Error("An error occured in PlayerAi::PlayerLook");
		throw 0;
	}
}

void PlayerAi::viewLog(){
	try{
		bool logMode = true;
		TCOD_key_t lastKey;

			engine.gui->ShowLog();
			while(logMode){
				TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
				switch(lastKey.vk){
					case TCODK_ESCAPE :logMode = false; break;
					default: break;
				}
			}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::viewLog");
		throw 0;
	}
}

void PlayerAi::Load(TCODZip &zip){
	try{
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::load");
		throw 0;
	}
}

void PlayerAi::Save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PlayerAi::save");
		throw 0;
	}
}
