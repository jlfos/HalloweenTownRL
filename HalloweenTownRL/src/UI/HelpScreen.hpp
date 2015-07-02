/*
 * HelpScreen.hpp
 *
 *  Created on: Jun 30, 2015
 */

#ifndef HELPSCREEN_HPP
#define HELPSCREEN_HPP

class HelpScreen {

public:
	HelpScreen();
	void Show();
	~HelpScreen();
private:
	TCODList<Gui::Message *> log;
};



#endif /* HELPSCREEN_HPP */
