/*
 * InventoryConsole.hpp
 *
 *  Created on: Jul 11, 2015

 */

#ifndef INVENTORYCONSOLE_HPP
#define INVENTORYCONSOLE_HPP
#include <vector>
#include <string>
#include "libtcod.hpp"
#include "ConsoleUI.hpp"

class Actor;

class InventoryConsole : public ConsoleUI{
public :
	InventoryConsole();
private:
	void userInput();
	Actor* getItem(int itemIndex);
	void useItem(Actor* item);
	Actor* player;
	std::map<char, int> keyMapping;
	void populateKeyMapping();


};



#endif /* INVENTORYCONSOLE_HPP */
