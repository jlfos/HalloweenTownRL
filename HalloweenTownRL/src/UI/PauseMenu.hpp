/*
 * PauseMenu.hpp
 *
 *  Created on: Mar 28, 2015
 */

#ifndef HALLOWEENTOWN_PAUSEMENU_HPP
#define HALLOWEENTOWN_PAUSEMENU_HPP

class PauseMenu : public Menu {
public:

	PauseMenu();
	void PopulateMenu(bool saveGameExists);
};



#endif /* HALLOWEENTOWN_PAUSEMENU_HPP */
