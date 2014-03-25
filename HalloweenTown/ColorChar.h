#pragma once
class ColorChar {
private:
	char character;
	int color;

public:
	ColorChar(char character, int i);
	ColorChar();
	ColorChar(char character);
	char getChar();
	void setChar(char c);
	int getColor();
	void setColor(int i);
	char setChar();
	enum Color {
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		Gray,
		Gold,
		DkGreen,
	};

};
