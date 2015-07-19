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
#include "ConsoleSelection.hpp"

class ConsoleFrame;
class ConsoleUI {
public:
	ConsoleUI(int width, int height, int startX, int startY);
	ConsoleUI(std::string text, int width, int startX, int startY);
	ConsoleUI(std::vector<Message> messages, int width, int startX, int startY);
	ConsoleUI(std::vector<ConsoleLine*> consoleLines, int width, int startX, int startY);
	~ConsoleUI();
	void display();
	void flush();
	void setConsoleLines(std::vector<ConsoleLine*> consoleLines);
	void clear();
	ConsoleFrame* frame;
	ConsoleSelection* selection;
private:
	std::vector<ConsoleLine*> consoleLines;
	TCODColor backgroundColor;
	TCODConsole* console;
	int startX;
	int startY;

};


#endif /* CONSOLEUI_HPP */
