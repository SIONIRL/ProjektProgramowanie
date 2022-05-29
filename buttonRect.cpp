#include "buttonRect.h"

buttonRect::buttonRect(char Type, int PosX, int PosY, int SzX, int SzY):
    button(Type, PosX, PosY), szX(SzX), szY(SzY) {}

buttonRect::~buttonRect() {}

bool buttonRect::hit(void)
{
	if( r_mseX < posX ) return false;
    if( r_mseX > posX + szX ) return false;
    if( r_mseY < posY ) return false;
    if( r_mseY > posY + szY ) return false;

	return true;
}

void buttonRect::set_label(const sf::Unicode::Text Label)
{
    label.SetText(Label);
    float fposX = static_cast<float>(posX);
	float fposY = static_cast<float>(posY);
	float fszX = static_cast<float>(szX);
	float fszY = static_cast<float>(szY);

	sf::FloatRect sR = label.GetRect();
	float len = sR.GetWidth();
	float hgt = sR.GetHeight();

	switch(labelPos)
	{
	case 'c':
		label.SetX( (2.0f*fposX + fszX - len)/2.0f );
		label.SetY( (2.0f*fposY + fszY - hgt)/2.0f - 1.5f );
		break;
	case 'l':
		label.SetX( fposX - len - 3.0f );
		label.SetY( (2.0f*fposY + fszY - hgt)/2.0f - 1.5f );
		break;
	case 'r':
		label.SetX( fposX +fszX + 3.0f );
		label.SetY( (2.0f*fposY + fszY - hgt)/2.0f - 1.5f );
		break;
	case 't':
		label.SetX( (2.0f*fposX + fszX - len)/2.0f );
		label.SetY( fposY - hgt - 3.0f );
		break;
	case 'b':
		label.SetX( (2.0f*fposX + fszX - len)/2.0f );
		label.SetY( fposY +fszY - 1.0f );
		break;
	default:
		break;
	}

	return;
}
