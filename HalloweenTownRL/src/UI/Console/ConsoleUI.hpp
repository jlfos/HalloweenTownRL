/*
 * ConsoleUI.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef CONSOLEUI_HPP
#define CONSOLEUI_HPP
#include <vector>
#include <string>
#include "libtcod.hpp"
#include "ConsoleLine.hpp"
class ConsoleUI {
public:
	ConsoleUI(int width, int height, int startX, int startY, bool frame, std::string frameTitle);
	ConsoleUI(std::vector<ConsoleLine*> consoleLines, TCODColor backgroundColor, int width, int height, int startX, int startY);
	ConsoleUI(std::vector<ConsoleLine*> consoleLines, TCODColor backgroundColor, int width, int height, int startX, int startY, bool frame, std::string frameTitle);
	void display();
	void setConsoleLines(std::vector<ConsoleLine*> consoleLines);
	bool incrementSelection();
	bool decrementSelection();
	int getSelection();
private:
	std::vector<ConsoleLine*> consoleLines;
	TCODColor backgroundColor;
	TCODConsole* console;
	int startX;
	int startY;
	int selection;
	bool frame;
	std::string frameTitle;

};


#endif /* CONSOLEUI_HPP */
