/*
 * ConsoleUI.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include <vector>
#include <string>
#include <iostream>
#include "libtcod.hpp"
#include "ConsoleLine.hpp"
#include "ConsoleUI.hpp"
#include "Rectangle.hpp"
#include "../../Tile/TileColors.hpp"

ConsoleUI::ConsoleUI(int width, int height, int startX, int startY, bool frame, bool selection, std::string frameTitle):
	 startX(startX), startY(startY), frame(frame), selectionFlag(selection), frameTitle(frameTitle){
	console = new TCODConsole(width, height);
}

ConsoleUI::ConsoleUI(std::vector<ConsoleLine*> consoleLines, TCODColor backgroundColor, int width, int height, int startX, int startY):
		ConsoleUI(consoleLines, backgroundColor, width, height, startX, startY, false, "")
{

}

ConsoleUI::ConsoleUI(std::vector<ConsoleLine*> consoleLines, TCODColor backgroundColor, int width, int height, int startX, int startY, bool frame, std::string frameTitle):
		consoleLines(consoleLines), backgroundColor(backgroundColor),  startX(startX), startY(startY), frame(frame), frameTitle(frameTitle){
	console = new TCODConsole(width, height);
}


void ConsoleUI::display(){
	console->clear();
	if(frame)
		console->printFrame(0, 0, console->getWidth(), console->getHeight(), true, TCOD_BKGND_DEFAULT, frameTitle.c_str());
		//console->printFrame(console->getWidth()-26, 0, console->getWidth()/2, console->getHeight(), true, TCOD_BKGND_DEFAULT, "Controls");
	int y  = 0, x  = 0;

	if(frame){
		y = 1;
		x = 1;
	}
	for(ConsoleLine* consoleLine : consoleLines ){
		for(Rectangle rectangle : consoleLine->getRectangle()){
			console->setDefaultBackground(rectangle.getColor());
			console->rect(x, y, rectangle.getWidth(), rectangle.getHeight(), false, TCOD_BKGND_SET);
		}
		if(selectionFlag && y == currentSelection){
			console->setDefaultBackground(TileColors::blue);
			console->rect(x, y, console->getWidth()-28, 1, false, TCOD_BKGND_SET);
		}
		//console->printEx(x, y, TCOD_BKGND_DEFAULT, TCOD_LEFT, consoleLine->getMessage().getText().c_str());
		console->print(x, y, consoleLine->getMessage().getText().c_str());
		y++;
	}

	console->setDefaultBackground(TileColors::black);
	TCODConsole::blit(console, 0, 0, console->getWidth(), console->getHeight(),TCODConsole::root, startX-10, startY);
	TCODConsole::flush();
}


void ConsoleUI::setConsoleLines(std::vector<ConsoleLine*> consoleLines){
	this->consoleLines = consoleLines;
	currentSelection = 1;
}


bool ConsoleUI::incrementSelection(){
	if(currentSelection+1>=consoleLines.size()+1)
		return false;
	else{
		currentSelection++;
		return true;
	}
}

bool ConsoleUI::decrementSelection(){

	if(currentSelection-1>0){
		currentSelection--;
		return true;
	}
	else
		return false;
}

int ConsoleUI::getSelection(){
	return currentSelection;
}


void ConsoleUI::clear(){
	console->clear();
	TCODConsole::flush();
}

ConsoleUI::~ConsoleUI(){
	delete console;
}
