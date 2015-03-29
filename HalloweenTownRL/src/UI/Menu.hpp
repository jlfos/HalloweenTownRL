#ifndef MENU_HPP_
#define MENU_HPP_

#include <string>

using namespace std;

class Menu{
public:
	enum class PauseMenuItemCode {
		NONE,
		NEW_GAME,
		CONTINUE,
		EXIT
	};
	virtual void populateMenu(bool saveGameExists) = 0;
	string pick();
	Menu();
	~Menu();
	void clear();

protected:
	void addItem(string item);
	void setHeader(string header);
private:
	string header;
	TCODList<string> items;
};


#endif
