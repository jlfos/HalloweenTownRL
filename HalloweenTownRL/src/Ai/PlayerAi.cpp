#include <iostream>
#include <chrono>
#include <thread>
#include "../Actor.hpp"
#include "../Gui.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/LevelUpMenu.hpp"
#include "../Pickable/Pickable.hpp"
#include "PlayerAi.hpp"


const int LEVEL_UP_BASE=1;
const int LEVEL_UP_INCREASE=20;

PlayerAi::PlayerAi() :experienceLevel(1), currentExperience(0), currentLevelGoal(LEVEL_UP_BASE) {

}

void PlayerAi::Update(Actor *owner){
	try{

		if(owner->destructible && owner->destructible->IsDead()){
			return;
		}
		switch(engine.lastKey.vk) {
		case TCODK_UP : MoveUp(owner); break;
		case TCODK_DOWN : MoveDown(owner); break;
		case TCODK_LEFT : MoveLeft(owner); break;
		case TCODK_RIGHT : MoveRight(owner); break;
		case TCODK_ESCAPE :LoadMenu(); break;
		case TCODK_CHAR : HandleActionKey(owner, engine.lastKey.c); break;
			default:break;
		}
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::Update" << std::endl;
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
		std::cerr << "An error occurred in PlayerAi::MoveLeft" << std::endl;
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
		std::cerr << "An error occurred in PlayerAi::MoveDown" << std::endl;
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
		std::cerr << "An error occurred in PlayerAi::MoveRight" << std::endl;
		throw 0;
	}
}

void PlayerAi::MoveUp(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (MoveOrAttack(owner, owner->x,owner->y-1)) {
			engine.currentMap->ComputeFov();
		}
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::MoveUp" << std::endl;
		throw 0;
	}
}

void PlayerAi::LoadMenu(){
	try{
		//engine.save();
		engine.Load();
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::LoadMenu" << std::endl;
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
			if(actor->destructible && !actor->destructible->IsDead() &&
					actor->x == targetX && actor->y == targetY){
				owner->attacker->Attack(owner, actor);
				if(actor->destructible->IsDead()){
					currentExperience += actor->destructible->ExperienceReward();
					if(LevelUpOccurred()){
						LevelUpPlayer(owner);
					}
				}
				return false;
			}
		}

		for(Actor* actor : engine.actors){
			bool corpseOrItem =(actor->destructible && actor->destructible->IsDead()) || actor->pickable;
			if(corpseOrItem && actor->x == targetX && actor->y == targetY){
				engine.gui->PushMessage(TCODColor::lightGrey,"There's a %s here",actor->name);
			}
		}
		owner->x=targetX;
		owner->y=targetY;
		return true;
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::MoveOrAttack" << std::endl;
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

		if(result == "Strength")
			player->attacker->SetPower(player->attacker->GetPower()+3 );

		else if(result == "Agility")
			player->destructible->SetDefense(player->destructible->GetDefense()+1 );

		else if(result == "Constitution")
			player->destructible->IncreaseTotalHealth(10);
	}
	catch(...){
		std::cerr << "An error occured in PlayerAi::LevelUpPlayer" << std::endl;
	}

}

bool PlayerAi::LevelUpOccurred(){
	return currentExperience > currentLevelGoal;
}

Actor *PlayerAi::ChooseFromInventory(Actor *owner){
	try{

		static const int INVENTORY_WIDTH=50;
		static const int INVENTORY_HEIGHT=28;
		static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);
		con.setDefaultForeground(TCODColor(200, 180, 50));
		con.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");

		con.setDefaultForeground(TCODColor::white);

		int shortcut = 'a';
		int y = 1;
		for(Actor *actor : owner->container->inventory){
			con.print(2, y, "(%c) %s", shortcut, actor->name);
			y++;
			shortcut++;
		}
		TCODConsole::blit(&con, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, TCODConsole::root,
					engine.screenWidth/2 - INVENTORY_WIDTH/2, engine.screenHeight/2 - INVENTORY_HEIGHT/2);
		TCODConsole::flush();

		TCOD_key_t key;
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

		if(key.vk == TCODK_CHAR){
			int actorIndex=key.c - 'a';
			if(actorIndex >= 0 && actorIndex < owner->container->inventory.size()){
				return owner->container->inventory.get(actorIndex);
			}
		}
		return nullptr;
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::chooseFromInventory" << std::endl;
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
					if ( actor->pickable && actor->x == owner->x && actor->y == owner->y ) {
						if (actor->pickable->Pick(actor,owner)) {
							found=true;
							engine.gui->PushMessage(TCODColor::lightGrey,"You pick the %s.",
								actor->name);
							break;
						} else if (! found) {
							found=true;
							engine.gui->PushMessage(TCODColor::red,"Your inventory is full.");
						}
					}
				}
				if (!found) {
					engine.gui->PushMessage(TCODColor::lightGrey,"There's nothing here that you can pick.");
				}
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'i' : // display inventory
			{
				Actor *actor=ChooseFromInventory(owner);
				if ( actor ) {
					actor->pickable->Use(actor,owner);
					engine.gameStatus=Engine::NEW_TURN;
				}
			}
			break;
			case '.' : //wait
			{
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'l':
			{
				PlayerLook(owner);
			}
			break;
		}
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::handleActionKey" << std::endl;
		throw 0;
	}
}

void PlayerAi::PlayerLook(Actor* player){
	bool lookMode = true;
	int cursorX = player->x;
	int cursorY = player->y;

	while(lookMode){
		engine.Render();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		TCODConsole::root->setChar(cursorX, cursorY, 219);
		TCODConsole::root->setCharForeground(cursorX, cursorY, TCODColor::white);
		TCODConsole::flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
		TCOD_key_t lastKey;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
		switch(lastKey.vk) {
		case TCODK_UP : cursorY--; break;
		case TCODK_DOWN : cursorY++; break;
		case TCODK_LEFT : cursorX--; break;
		case TCODK_RIGHT : cursorX++; break;
		case TCODK_ESCAPE :lookMode = false; break;
//		case TCODK_CHAR : handleActionKey(owner, engine.lastKey.c); break;
			default:break;
		}


	}
}

void PlayerAi::Load(TCODZip &zip){
	try{
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::load" << std::endl;
		throw 0;
	}
}

void PlayerAi::Save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
	}
	catch(...){
		std::cerr << "An error occurred in PlayerAi::save" << std::endl;
		throw 0;
	}
}
