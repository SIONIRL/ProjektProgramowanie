#include "dragDropRect.h"

dragDropRect::dragDropRect(int PosX, int PosY, sf::Sprite* p_Sprite): dragDrop(PosX, PosY, p_Sprite)
{

}
dragDropRect::~dragDropRect() {}
void dragDropRect::assign_Size(sf::Sprite* p_Sprite)
{
    if( p_Sprite )
        pSprite = p_Sprite;

    const sf::Image* pImg = pSprite->GetImage();
    if( pImg )
    {
        szX = pImg->GetWidth();
        szY = pImg->GetHeight();

        if( p_anchorPos && homeIdx < Nanchors && homeIdx >= 0 )
        {
            posX = p_anchorPos[homeIdx].first - szX/2;
            posY = p_anchorPos[homeIdx].second - szY/2;
            docked = true;
        }
    }
}
bool dragDropRect::hit(void)
{
    if( r_mseX < posX ) return false;
    if( r_mseX > posX + szX ) return false;
    if( r_mseY < posY ) return false;
    if( r_mseY > posY + szY ) return false;
	return true;
}
void dragDropRect::snap(void)
{
    if( !held && !docked )
    {
        posX = p_anchorPos[homeIdx].first - szX/2;
        posY = p_anchorPos[homeIdx].second - szY/2;
        docked = true;
    }
}
bool dragDropRect::hitAnchor(int idx)
{
    if( p_anchorPos[idx].first < posX ) return false;
    if( p_anchorPos[idx].first > posX + szX ) return false;
    if( p_anchorPos[idx].second < posY ) return false;
    if( p_anchorPos[idx].second > posY + szY ) return false;

	return true;
}
