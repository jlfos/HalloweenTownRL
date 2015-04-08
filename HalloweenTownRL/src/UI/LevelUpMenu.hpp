/*
 * LevelUpMenu.hpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */

#ifndef LEVELUPMENU_HPP_
#define LEVELUPMENU_HPP_


class LevelUpMenu : public Menu {
public:
	LevelUpMenu();
	void PopulateMenu(bool saveGameExists);
};



#endif /* LEVELUPMENU_HPP_ */
