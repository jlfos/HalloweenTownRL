/*
 * ConsoleUI.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include <vector>
#include <string>
#include <iostream>
#include "libtcod.hpp"
#include "ConsoleFrame.hpp"
#include "ConsoleLine.hpp"
#include "ConsoleUI.hpp"
#include "Rectangle.hpp"
#include "../../Tile/TileColors.hpp"

ConsoleUI::ConsoleUI(int width, int height, int startX, int startY):
	 startX(startX), startY(startY){
	console = new TCODConsole(width, height);
	frame = nullptr;
	selection = nullptr;
}

ConsoleUI::ConsoleUI(std::string text, int width, int startX, int startY): startX(startX), startY(startY) {
	frame = nullptr;
	selection = nullptr;
	this->consoleLines = ConsoleLine::createConsoleLines(text, width);
	console = new TCODConsole(width, this->consoleLines.size()+2);
}

ConsoleUI::ConsoleUI(std::vector<Message> messages, int width, int startX,
		int startY) : startX(startX), startY(startY) {
	frame = nullptr;
	selection = nullptr;
	this->consoleLines = ConsoleLine::createConsoleLines(messages);
	console = new TCODConsole(width, this->consoleLines.size()+2);
}

ConsoleUI::ConsoleUI(std::vector<ConsoleLine*> consoleLines, int width,
		int startX, int startY) : startX(startX), startY(startY) {
	frame = nullptr;
	selection = nullptr;
	this->consoleLines = consoleLines;
	console = new TCODConsole(width, consoleLines.size()+2);
}

ConsoleUI::~ConsoleUI(){
	delete console;
	if(frame)
		delete frame;
	if(selection)
		delete selection;
}

void ConsoleUI::display(){
	console->clear();
	int y  = 0, x  = 0;
	if(frame){
		console->setDefaultForeground(frame->getFrameColor());
		console->printFrame(0, 0, console->getWidth(), console->getHeight(), true, TCOD_BKGND_DEFAULT, frame->getFrameTitle().c_str());
		y = 1;
		x = 1;
	}


	for(ConsoleLine* consoleLine : consoleLines ){
		for(Rectangle rectangle : consoleLine->getRectangle()){
			console->setDefaultBackground(rectangle.getColor());
			console->rect(x, y, rectangle.getWidth(), rectangle.getHeight(), false, TCOD_BKGND_SET);
		}
		if(selection && y == selection->getSelection()){
			console->setDefaultBackground(selection->getColor());
			console->rect(x, y, console->getWidth()-2, 1, false, TCOD_BKGND_SET);
		}
		console->print(x, y, consoleLine->getMessage().getText().c_str());
		y++;
	}

	console->setDefaultBackground(TileColors::black);
	TCODConsole::blit(console, 0, 0, console->getWidth(), console->getHeight(),TCODConsole::root, startX-10, startY);
}


void ConsoleUI::setConsoleLines(std::vector<ConsoleLine*> consoleLines){
	this->consoleLines = consoleLines;
}



void ConsoleUI::clear(){
	console->clear();
	TCODConsole::flush();
}

void ConsoleUI::flush() {
	TCODConsole::flush();
}
