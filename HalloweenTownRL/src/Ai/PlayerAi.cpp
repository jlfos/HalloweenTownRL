#include <iostream>
#include <chrono>
#include <thread>
#include "../main.hpp"

using namespace std;


const int LEVEL_UP_BASE=1;
const int LEVEL_UP_INCREASE=20;

PlayerAi::PlayerAi() :experienceLevel(1), currentExperience(0), currentLevelGoal(LEVEL_UP_BASE) {

}

void PlayerAi::update(Actor *owner){
	try{

		if(owner->destructible && owner->destructible->isDead()){
			return;
		}
		switch(engine.lastKey.vk) {
		case TCODK_UP : MoveUp(owner); break;
		case TCODK_DOWN : MoveDown(owner); break;
		case TCODK_LEFT : MoveLeft(owner); break;
		case TCODK_RIGHT : MoveRight(owner); break;
		case TCODK_ESCAPE :LoadMenu(); break;
		case TCODK_CHAR : handleActionKey(owner, engine.lastKey.c); break;
			default:break;
		}
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::update" << endl;
		throw 0;
	}

}


void PlayerAi::MoveLeft(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x-1,owner->y)) {
			engine.currentMap->computeFov();
		}
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::MoveLeft" << endl;
		throw 0;
	}
}

void PlayerAi::MoveDown(Actor *owner){
    try{
		engine.gameStatus=Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x,owner->y+1)) {
			engine.currentMap->computeFov();
		}
    }
    catch(...){
		cerr << "An error occurred in PlayerAi::MoveDown" << endl;
		throw 0;
    }
}


void PlayerAi::MoveRight(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x+1,owner->y)) {
			engine.currentMap->computeFov();
		}
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::MoveRight" << endl;
		throw 0;
	}
}

void PlayerAi::MoveUp(Actor *owner){
	try{
		engine.gameStatus=Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x,owner->y-1)) {
			engine.currentMap->computeFov();
		}
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::MoveUp" << endl;
		throw 0;
	}
}

void PlayerAi::LoadMenu(){
	try{
		//engine.save();
		engine.load();
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::LoadMenu" << endl;
		throw 0;
	}
}
bool PlayerAi::moveOrAttack(Actor *owner, int targetX, int targetY){
	try{
		Map::TileType type = engine.currentMap->getTileType(targetX, targetY);
		if(type == Map::TileType::WALL)  return false;
		else if(type == Map::TileType::TOP_EDGE ||
				type == Map::TileType::RIGHT_EDGE ||
				type == Map::TileType::BOTTOM_EDGE ||
				type == Map::TileType::LEFT_EDGE) {
				engine.nextLevel(type);
				return false;
		}
		for(Actor* actor : engine.actors){
			if(actor->destructible && !actor->destructible->isDead() &&
					actor->x == targetX && actor->y == targetY){
				owner->attacker->attack(owner, actor);
				if(actor->destructible->isDead()){
					currentExperience += actor->destructible->getExperienceReward();
					if(levelUpOccurred()){
						levelUpPlayer(owner);
					}
				}
				return false;
			}
		}

		for(Actor* actor : engine.actors){
			bool corpseOrItem =(actor->destructible && actor->destructible->isDead()) || actor->pickable;
			if(corpseOrItem && actor->x == targetX && actor->y == targetY){
				engine.gui->message(TCODColor::lightGrey,"There's a %s here",actor->name);
			}
		}
		owner->x=targetX;
		owner->y=targetY;
		return true;
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::moveOrAttack" << endl;
		throw 0;
	}
}

int PlayerAi::getLevel(){
	return experienceLevel;
}

void PlayerAi::levelUpPlayer(Actor* player){
	try{
		experienceLevel++;
		currentLevelGoal += LEVEL_UP_INCREASE;
		LevelUpMenu levelUpMenu;
		string result = levelUpMenu.pick();

		if(result == "Strength")
			player->attacker->setPower(player->attacker->getPower()+3 );

		else if(result == "Agility")
			player->destructible->setDefense(player->destructible->getDefense()+1 );

		else if(result == "Constitution")
			player->destructible->increaseTotalHealth(10);
	}
	catch(...){
		cerr << "An error occured in PlayerAi::levelUpPlayer" << endl;
	}

}

bool PlayerAi::levelUpOccurred(){
	return currentExperience > currentLevelGoal;
}

Actor *PlayerAi::choseFromInventory(Actor *owner){
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
		cerr << "An error occurred in PlayerAi::chooseFromInventory" << endl;
		throw 0;
	}
}

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
	try
	{
			switch(ascii) {

			case 'g' : // pickup item
			{
				bool found=false;
				for (Actor *actor : engine.actors) {
					if ( actor->pickable && actor->x == owner->x && actor->y == owner->y ) {
						if (actor->pickable->pick(actor,owner)) {
							found=true;
							engine.gui->message(TCODColor::lightGrey,"You pick the %s.",
								actor->name);
							break;
						} else if (! found) {
							found=true;
							engine.gui->message(TCODColor::red,"Your inventory is full.");
						}
					}
				}
				if (!found) {
					engine.gui->message(TCODColor::lightGrey,"There's nothing here that you can pick.");
				}
				engine.gameStatus=Engine::NEW_TURN;
			}
			break;
			case 'i' : // display inventory
			{
				Actor *actor=choseFromInventory(owner);
				if ( actor ) {
					actor->pickable->use(actor,owner);
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
				playerLook(owner);
			}
			break;
		}
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::handleActionKey" << endl;
		throw 0;
	}
}

void PlayerAi::playerLook(Actor* player){
	bool lookMode = true;
	int cursorX = player->x;
	int cursorY = player->y;

	while(lookMode){
		engine.render();
		this_thread::sleep_for(chrono::milliseconds(1));
		TCODConsole::root->setChar(cursorX, cursorY, 219);
		TCODConsole::root->setCharForeground(cursorX, cursorY, TCODColor::white);
		TCODConsole::flush();
		this_thread::sleep_for(chrono::milliseconds(3));
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

void PlayerAi::load(TCODZip &zip){
	try{
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::load" << endl;
		throw 0;
	}
}

void PlayerAi::save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
	}
	catch(...){
		cerr << "An error occurred in PlayerAi::save" << endl;
		throw 0;
	}
}
