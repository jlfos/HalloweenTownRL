#ifndef MENU_HPP_
#define MENU_HPP_

#include <string>

class Menu{
public:


	enum class PauseMenuItemCode {
		NONE,
		NEW_GAME,
		CONTINUE,
		EXIT
	};
	virtual void PopulateMenu(bool saveGameExists) = 0;
	std::string Pick();
	Menu();
	~Menu();
	void Clear();

protected:
	void AddItem(std::string item);
	void SetHeader(std::string header);
private:
	std::string header;
	TCODList<std::string> items;
};


#endif
