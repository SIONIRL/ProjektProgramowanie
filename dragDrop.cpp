#include "dragDrop.h"

sf::RenderWindow* dragDrop::pWndw = NULL;
float dragDrop::snap_speed = 30.0f;

dragDrop::dragDrop(int PosX, int PosY, sf::Sprite* p_Sprite): pSprite(p_Sprite), posX(PosX), posY(PosY)
{
    grabX = grabY = 0;
    held = false;
    docked = true;

    p_anchorPos = NULL;
    Nanchors = 0;
    homeIdx = 0;
}

dragDrop::~dragDrop() {}

void dragDrop::set_anchors(std::pair<int, int>* p_AnchorArr, int arrSize, int HomeIdx)
{
    p_anchorPos = p_AnchorArr;
    Nanchors = arrSize;
    homeIdx = HomeIdx;
}

void dragDrop::draw(void)
{
	pSprite->SetPosition( static_cast<float>(posX), static_cast<float>(posY) );

	if( pWndw )
		pWndw->Draw( *pSprite );

	return;
}

bool dragDrop::grab(void)
{
    if( hit() )
    {
        held = true;
        docked = false;
        grabX = r_mseX - posX;
        grabY = r_mseY - posY;
    }
    return held;
}

void dragDrop::release(void)
{
    if( held )
    {
        held = false;
        if( p_anchorPos )
        {
            for(int i=0; i<Nanchors; ++i)
                if( i != homeIdx && hitAnchor(i) )
                {
                    homeIdx = i;
                    break;
                }
        }
    }
    return;
}

void dragDrop::release( int* p_IdxList, int listSz )
{
    if( held )
    {
        held = false;
        if( p_anchorPos )
        {
            docked = false;
            for(int i=0; i<listSz; ++i)
                if( hitAnchor( p_IdxList[i] ) )
                {
                    homeIdx = p_IdxList[i];
                    break;
                }
        }
    }
    return;
}

void dragDrop::drag(void)
{
    if( held )
    {
        posX = r_mseX - grabX;
        posY = r_mseY - grabY;
    }
}

