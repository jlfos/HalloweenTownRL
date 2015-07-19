/*
 * ConsoleLine.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef CONSOLELINE_HPP
#define CONSOLELINE_HPP
#include <vector>
#include "libtcod.hpp"
#include "Message.hpp"
#include "Rectangle.hpp"

class ConsoleLine{

public:
	ConsoleLine(Message message);
	ConsoleLine(Message message, std::vector<Rectangle> rectangle);
	Message getMessage();
	std::vector<Rectangle> getRectangle();
	static std::vector<ConsoleLine*> createConsoleLines(std::vector<Message> messages);
	static std::vector<ConsoleLine*> createConsoleLines(std::string text, unsigned int lineWidth);
private:
	Message message;
	std::vector<Rectangle> rectangles;
};



#endif /* CONSOLELINE_HPP */
