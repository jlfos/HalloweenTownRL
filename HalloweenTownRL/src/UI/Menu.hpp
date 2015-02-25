//#ifndef MENU_HPP_
//#define MENU_HPP_


class Menu{
public:
	enum MenuItemCode {
		NONE,
		NEW_GAME,
		CONTINUE,
		EXIT
	};
	void populateMenu(bool saveGameExists);
	MenuItemCode pick();
	Menu();
	~Menu();
	void clear();
private:
	void addItem(MenuItemCode code, const char *label);
protected:

	struct MenuItem{
		MenuItemCode code;
		const char *label;
	};
	TCODList<MenuItem *> items;
};

//
//#endif
