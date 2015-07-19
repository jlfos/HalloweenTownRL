/*
 * VictoryScreen.hpp
 *
 *  Created on: Jul 2, 2015
 */

#ifndef VICTORYSCREEN_HPP
#define VICTORYSCREEN_HPP
#include <string>
#include "ScreenUI.hpp"

class VictoryScreen : public ScreenUI{

public:
	VictoryScreen();
private:
	std::string text;
};



#endif /* VICTORYSCREEN_HPP */
