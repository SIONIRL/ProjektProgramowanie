#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class button
{
public:
	static sf::RenderWindow* pWndw;
	static const int& r_mseX;
	static const int& r_mseY;
	sf::String label;
	char labelPos;

	int posX;
	int posY;
	bool sel;
	bool mo;
	char type;

	

	
	virtual bool hit(void) = 0;
	virtual void draw(void) = 0;
    virtual void set_label(const sf::Unicode::Text Label) = 0;

	
	bool mseOver(void);
	bool hit_dn(void);
	void hit_up(void);

	
	void INITlabel(const sf::Unicode::Text Label, int fontSize, sf::Color txtColor, char LabelPos);

	
	button(void);
	button(char Type, int PosX, int PosY);

	
	virtual ~button(void);
};

#endif
