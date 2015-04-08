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
	virtual void PopulateMenu(bool saveGameExists) = 0;
	string Pick();
	Menu();
	~Menu();
	void Clear();

protected:
	void AddItem(string item);
	void SetHeader(string header);
private:
	string header;
	TCODList<string> items;
};


#endif
