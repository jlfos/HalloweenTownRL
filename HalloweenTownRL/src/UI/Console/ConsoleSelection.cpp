/*
 * ConsoleSelection.cpp
 *
 *  Created on: Jul 18, 2015
 */
#include "libtcod.hpp"
#include "ConsoleSelection.hpp"


ConsoleSelection::ConsoleSelection(int max, TCODColor color) : color(color), selection(1), max(max) {
}

int ConsoleSelection::getSelection(){
	return selection;
}

bool ConsoleSelection::incrementSelection() {
	if(selection+1>=max+1)
		return false;
	else{
		selection++;
		return true;
	}
}



bool ConsoleSelection::decrementSelection() {
	if(selection-1>0){
		selection--;
		return true;
	}
	else
		return false;
}

TCODColor ConsoleSelection::getColor() const {
	return color;
}
