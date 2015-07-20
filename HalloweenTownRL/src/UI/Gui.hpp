#ifndef HALLOWEENTOWN_GUI_HPP
#define HALLOWEENTOWN_GUI_HPP
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include "Console/Message.hpp"
#include "Console/ConsoleUI.hpp"
#include "PauseMenu.hpp"
#include "../Persistent.hpp"

class ConsoleUI;

class Gui : public Persistent {
public :
	PauseMenu* menu;
	Gui();
	~Gui();
	void Render();
	void PushMessage(const TCODColor &col, const char *text, ...);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
	void Clear();
	void ShowLog();
	void PopulatePauseMenu(bool saveFileExists);
	std::string PauseMenuPick();
	void PauseMenuClear();
	void setCharAdjusted(int x, int y, int c);
	void setForegroundAdjusted(int x, int y, TCODColor color);
protected :
	TCODConsole *con;
	float colorCoef;

	std::unique_ptr<std::deque<Message>> log;

	void RenderBar(int x, int y, int width, const char *name,
		float value, float maxValue, const TCODColor &barColor,
		const TCODColor &backColor);
	void RenderMouseLook();
private:
	ConsoleUI *activeLog;
	std::vector<Message> getActiveLog();
};
#endif
