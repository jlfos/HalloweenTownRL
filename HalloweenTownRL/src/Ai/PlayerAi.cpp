#include "../main.hpp"

void PlayerAi::update(Actor *owner){
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


void PlayerAi::MoveLeft(Actor *owner){
    engine.gameStatus=Engine::NEW_TURN;
    if (moveOrAttack(owner, owner->x-1,owner->y)) {
        engine.map->computeFov();
    }
}

void PlayerAi::MoveDown(Actor *owner){
    engine.gameStatus=Engine::NEW_TURN;
    if (moveOrAttack(owner, owner->x,owner->y+1)) {
        engine.map->computeFov();
    }
}


void PlayerAi::MoveRight(Actor *owner){
    engine.gameStatus=Engine::NEW_TURN;
    if (moveOrAttack(owner, owner->x+1,owner->y)) {
        engine.map->computeFov();
    }
}

void PlayerAi::MoveUp(Actor *owner){
    engine.gameStatus=Engine::NEW_TURN;
    if (moveOrAttack(owner, owner->x,owner->y-1)) {
        engine.map->computeFov();
    }
}

void PlayerAi::LoadMenu(){
	engine.save();
	engine.load();
}
bool PlayerAi::moveOrAttack(Actor *owner, int targetX, int targetY){
	if(engine.map->isWall(targetX, targetY)) return false;

	for(Actor* actor : engine.actors){
		if(actor->destructible && !actor->destructible->isDead() &&
				actor->x == targetX && actor->y == targetY){
			owner->attacker->attack(owner, actor);
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

Actor *PlayerAi::choseFromInventory(Actor *owner){
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

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
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
	}
}

void PlayerAi::load(TCODZip &zip){
}

void PlayerAi::save(TCODZip &zip){
	zip.putInt(PLAYER);
}
