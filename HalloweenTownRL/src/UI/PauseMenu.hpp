/*
 * PauseMenu.hpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */

#ifndef PAUSEMENU_HPP_
#define PAUSEMENU_HPP_

class PauseMenu : public Menu {
public:

	PauseMenu();
	void populateMenu(bool saveGameExists);
};



#endif /* PAUSEMENU_HPP_ */
