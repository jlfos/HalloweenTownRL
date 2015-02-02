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

	void clear();
	void addItem(MenuItemCode code, const char *label);
	MenuItemCode pick();
	~Menu();
protected:

	struct MenuItem{
		MenuItemCode code;
		const char *label;
	};
	TCODList<MenuItem *> items;
};

//
//#endif
