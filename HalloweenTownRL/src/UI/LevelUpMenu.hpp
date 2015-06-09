/*
 * LevelUpMenu.hpp
 *
 *  Created on: Mar 28, 2015
 */

#ifndef HALLOWEENTOWN_LEVELUPMENU_HPP
#define HALLOWEENTOWN_LEVELUPMENU_HPP


class LevelUpMenu : public Menu {
public:
	LevelUpMenu();
	void PopulateMenu(bool saveGameExists);
};



#endif /* HALLOWEENTOWN_LEVELUPMENU_HPP */
