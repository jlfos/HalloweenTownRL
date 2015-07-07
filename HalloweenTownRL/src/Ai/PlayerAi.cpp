#include <iostream>
#include <chrono>
#include <thread>
#include "../Actor/Actor.hpp"
#include "../UI/Gui.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/HelpScreen.hpp"
#include "../UI/LevelUpMenu.hpp"
#include "../UI/MonsterLogMenu.hpp"
#include "../Item/Item.hpp"
#include "../Tile/TileColors.hpp"
#include "PlayerAi.hpp"


const int LEVEL_UP_BASE=1;
const int LEVEL_UP_INCREASE=20;

PlayerAi::PlayerAi() :experienceLevel(1), currentExperience(0), currentLevelGoal(LEVEL_UP_BASE) {

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
			bool corpseOrItem =(actor->destructible && actor->destructible->IsDead()) || actor->item;
			if(corpseOrItem && actor->x == targetX && actor->y == targetY){
				engine.gui->PushMessage(TileColors::lightGrey,"There's a %s here", (actor->name).c_str() );
			}
		}
//		engine.currentMap->computeLight(owner, false, engine.fovRadius);
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
		engine.gui->PushMessage(TileColors::blue,"You leveled up! You are now level %d.", experienceLevel );
		if(result == "Strength")
			player->attacker->setBasePower(player->attacker->getBasePower()+3 );

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

//TODO move this functionality to the GUI
Actor *PlayerAi::ChooseFromInventory(Actor *owner){
	try{

		static const int INVENTORY_WIDTH=50;
		static const int INVENTORY_HEIGHT=28;
		static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);
		con.setDefaultForeground(TileColors::darkYellow);
		con.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");

		con.setDefaultForeground(TileColors::white);

		int shortcut = 'a';
		int y = 1;
		for(Actor *actor : owner->container->inventory){
			con.print(2, y, "(%c) %s", shortcut, (actor->name).c_str());
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

void PlayerAi::showHelp() {
	HelpScreen help;
	help.Show();
	bool logMode = true;
	TCOD_key_t lastKey;
	while (logMode) {
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
		switch (lastKey.vk) {
		case TCODK_ESCAPE:
			logMode = false;
			break;
		default:
			break;
		}
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
							engine.gui->PushMessage(TileColors::lightGrey,"You picked up the %s.",
								(actor->name).c_str());
							break;
						} else if (! found) {
							found=true;
							engine.gui->PushMessage(TileColors::red,"Your inventory is full.");
						}
					}
				}
				if (!found) {
					engine.gui->PushMessage(TileColors::lightGrey,"There's nothing here that you can pick up.");
				}
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'i' : // display inventory
			{
				Actor *actor=ChooseFromInventory(owner);
				if ( actor ) {
					actor->item->Use(actor,owner);
					engine.gameStatus=Engine::NEW_TURN;
				}
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
				showHelp();
			}
			break;
			case 'm' :
			{
				MonsterLogMenu monsterLog;
				monsterLog.PopulateMenu(false);
				std::string result = monsterLog.Pick();

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
		TCODConsole::root->setCharForeground(cursorX, cursorY, TileColors::white);
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
		case TCODK_ENTER :
			//TODO break this out, its used elsewhere
			for(Actor* actor : engine.actors){
				if(actor->x == cursorX && actor->y == cursorY){

					engine.gui->PushMessage(TileColors::grey, "A %s", actor->name.c_str());
				}
			}

			break;
//		case TCODK_CHAR : handleActionKey(owner, engine.lastKey.c); break;
			default:break;
		}


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
		std::cerr << "An error occurred in PlayerAi::viewLog" << std::endl;
		throw 0;
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
