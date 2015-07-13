#ifndef HALLOWEENTOWN_GUI_HPP
#define HALLOWEENTOWN_GUI_HPP
#include <string>
#include <vector>
#include "PauseMenu.hpp"
#include "../Persistent.hpp"
#include "Console/Message.hpp"
class Gui : public Persistent {
public :
	PauseMenu menu;
	Gui();
	~Gui();
	void Render();
	void PushMessage(const TCODColor &col, const char *text, ...);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
	void Clear();
	void ShowLog();
	static std::vector<Message> wordWrapText(std::string, int lineSize);

protected :
	TCODConsole *con;


	TCODList<Message *> log;

	void RenderBar(int x, int y, int width, const char *name,
		float value, float maxValue, const TCODColor &barColor,
		const TCODColor &backColor);
	void RenderMouseLook();
};
#endif
