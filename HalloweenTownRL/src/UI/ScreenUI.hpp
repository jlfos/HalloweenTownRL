/*
 * ScreenUI.hpp
 *
 *  Created on: Jul 18, 2015
 */

#ifndef SCREENUI_HPP
#define SCREENUI_HPP
#include <string>
#include "Console/Message.hpp"
class ScreenUI {
public:
	ScreenUI();
	~ScreenUI();
	void Show();
protected:
	std::vector<Message>* log;
};



#endif /* SCREENUI_HPP */
