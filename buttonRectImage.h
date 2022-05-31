#ifndef BUTTONRECTIMAGE_H
#define BUTTONRECTIMAGE_H

#include "buttonRect.h"


class buttonRectImage : public buttonRect
{
    public:
	sf::Sprite* pSprite;

	virtual void draw(void);
	void assign_Sprite(sf::Sprite* p_Sprite);

	buttonRectImage(char Type, int PosX, int PosY, sf::Sprite* p_Sprite);
    ~buttonRectImage();
};

#endif
