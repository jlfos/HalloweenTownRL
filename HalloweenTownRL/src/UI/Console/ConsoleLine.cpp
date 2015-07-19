/*
 * ConsoleLine.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include <vector>
#include "ConsoleLine.hpp"
#include "Message.hpp"
#include "Rectangle.hpp"

ConsoleLine::ConsoleLine(Message message, std::vector<Rectangle> rectangles): message(message), rectangles(rectangles){

}

ConsoleLine::ConsoleLine(Message message): message(message) {
	this->rectangles = std::vector<Rectangle>();
}

Message ConsoleLine::getMessage(){
	return message;
}

std::vector<Rectangle> ConsoleLine::getRectangle(){
	return rectangles;
}

/**
 * Takes a vector of messages and returns a vector of consoleline pointers
 */
std::vector<ConsoleLine*> ConsoleLine::createConsoleLines(std::vector<Message> messages){
	std::vector<ConsoleLine*> consoleLines;
	for(Message m : messages){
		ConsoleLine* cl = new ConsoleLine(m);
		consoleLines.push_back(cl);
	}
	return consoleLines;

}

/**
 * Takes a string of text, word wraps it (via Message::wordWrapText) and returns a vector
 * of consoleline pointers.
 */
std::vector<ConsoleLine*> ConsoleLine::createConsoleLines(std::string text, unsigned int lineSize) {
	std::vector<Message> messages = Message::wordWrapText(text, lineSize);
	return createConsoleLines(messages);
}
