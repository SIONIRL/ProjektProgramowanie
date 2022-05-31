#ifndef DRAGDROPRECT_H
#define DRAGDROPRECT_H
#include "dragDrop.h"
class dragDropRect : public dragDrop
{
public:
    int szX;
    int szY;
    bool hit(void);
    void snap(void);
    bool hitAnchor(int idx);
    void assign_Size(sf::Sprite* p_Sprite = NULL);

    dragDropRect(int PosX, int PosY, sf::Sprite* p_Sprite);
    ~dragDropRect();
};

#endif
