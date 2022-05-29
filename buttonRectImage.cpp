#include "buttonRectImage.h"

buttonRectImage::buttonRectImage(char Type, int PosX, int PosY, sf::Sprite* p_Sprite ):
    buttonRect(Type, PosX, PosY, 10, 10), pSprite(p_Sprite)
    {
        if( pSprite )
        {
            const sf::Image* pImg = pSprite->GetImage();
            if( pImg )
            {
                szX = pImg->GetWidth()/2;
                szY = pImg->GetHeight()/2;
            }
        }
    }

buttonRectImage::~buttonRectImage() {}

void buttonRectImage::assign_Sprite(sf::Sprite* p_Sprite)
{
    pSprite = p_Sprite;
    const sf::Image* pImg = pSprite->GetImage();
    if( pImg )
    {
        szX = pImg->GetWidth()/2;
        szY = pImg->GetHeight()/2;
    }
}

void buttonRectImage::draw(void)
{
	sf::IntRect srcRect;
	srcRect.Top = sel ? szY+1 : 0;
	srcRect.Left = mo ? szX+1 : 0;
	srcRect.Bottom = srcRect.Top  + szY;
	srcRect.Right = srcRect.Left + szX;
	pSprite->SetSubRect(srcRect);
	pSprite->SetPosition( static_cast<float>(posX), static_cast<float>(posY) );

	if( pWndw )
	{
		pWndw->Draw( *pSprite );
		pWndw->Draw( label );
	}
	return;
}
