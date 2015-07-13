/*
 * MonsterLogMenu.hpp
 *
 *  Created on: Jun 30, 2015
 */

#ifndef MONSTERLOG_HPP
#define MONSTERLOG_HPP
#include "libtcod.hpp"
#include "Gui.hpp"
#include "MenuUI.hpp"

class Message;

class MonsterLogMenu : public MenuUI {
	public:
	MonsterLogMenu();
	void PopulateMenu(bool hardMode);
	private:
	TCODList<Message*> log;
};



#endif /* MONSTERLOGMENU_HPP */
