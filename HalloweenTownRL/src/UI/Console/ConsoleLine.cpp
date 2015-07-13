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


