/*
 * ConsoleSelection.hpp
 *
 *  Created on: Jul 18, 2015
 */

#ifndef CONSOLESELECTION_HPP
#define CONSOLESELECTION_HPP
#include "libtcod.hpp"

class ConsoleSelection{
public:
	ConsoleSelection(int max, TCODColor color);
	int getSelection();
	bool incrementSelection();
	bool decrementSelection();
	TCODColor getColor() const;

private:
	TCODColor color; //the color to highlight the current selection with
	int selection; //index of currently selected item
	int max; //maximum value the index cannot exceed
};



#endif /* CONSOLESELECTION_HPP */
