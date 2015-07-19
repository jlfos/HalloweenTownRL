#ifndef HALLOWEENTOWN_MENUUI_HPP
#define HALLOWEENTOWN_MENUUI_HPP

#include <string>
#include "libtcod.hpp"

class MenuUI{
public:


	enum class PauseMenuItemCode {
		NONE,
		NEW_GAME,
		CONTINUE,
		EXIT
	};
	std::string Pick();
	MenuUI();
	~MenuUI();
	void Clear();

protected:
	void AddItem(std::string item);
	void SetHeader(std::string header);
private:
	std::string header;
	TCODList<std::string> items;
};


#endif
