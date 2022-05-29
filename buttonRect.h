#ifndef BUTTONRECT_H
#define BUTTONRECT_H

#include "button.h"

class buttonRect : public button
{
public:
    int szX;
	int szY;

	
	virtual bool hit(void);
	virtual void draw(void) = 0;
	void set_label(const sf::Unicode::Text Label);

	
	buttonRect(char Type, int Posx, int PosY, int SzX, int SzY);
	~buttonRect();
};

#endif
