#ifndef BUTTONRECTDRAWN_H
#define BUTTONRECTDRAWN_H

#include "buttonRect.h"


class buttonRectDrawn :	public buttonRect
{
public:

	sf::Color selColor;
	sf::Color unSelColor;
	sf::Color moColor;
	sf::Color unMoColor;

	// functions
	virtual void draw(void);
	void set_colors(sf::Color unSel_color, sf::Color sel_color, sf::Color unMo_color, sf::Color mo_color);

	// constructors
	buttonRectDrawn(char Type, int Posx, int PosY, int SzX, int SzY);
	buttonRectDrawn();

	// destructor
	~buttonRectDrawn(void);
};

#endif // BUTTONRECTDRAWN_H
