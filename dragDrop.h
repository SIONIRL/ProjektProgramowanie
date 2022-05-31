#ifndef DRAGDROP_H
#define DRAGDROP_H

#include <SFML/Graphics.hpp>
#include <vector>

class dragDrop
{
public:
    static sf::RenderWindow* pWndw;
	const static int& r_mseX;
	const static int& r_mseY;
	static float snap_speed;

    sf::Sprite* pSprite;
	int posX;
	int posY;
	int grabX;
	int grabY;
	bool held;
	bool docked;

	std::pair<int, int>* p_anchorPos;
    int Nanchors;
    int homeIdx;

	virtual bool hit(void) = 0;
	virtual bool hitAnchor(int idx) = 0;
	virtual void snap(void) = 0;

	void draw(void);
	bool grab(void);
	void release(void);
	void release( int* p_IdxList, int listSz );
	void drag(void);

	void set_anchors(std::pair<int, int>* p_AnchorArr, int arrSize, int homeIdx);

    dragDrop(int PosX, int PosY, sf::Sprite* p_Sprite);
    virtual ~dragDrop();
};

#endif
