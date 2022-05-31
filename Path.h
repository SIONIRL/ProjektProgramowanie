#ifndef PATH_H
#define PATH_H

#include <SFML/Graphics.hpp>
#include "Leg.h"


class Path
{
    public:

    static sf::RenderWindow* pWndw;
    sf::Sprite** ppSprite;

    
    Leg** ppLeg;
    float xi;
    float yi;
    float* px;
    float* py;
    float* ptLeg;
    int nLegs;
    int nLegs_inUse;

    
    bool inUse;
    bool done;
    int delay;
    int launch_dt;

    
    void reset(bool InUse, int Delay = 0);
    void move();
    void draw();
    Path(int N_legs, int Delay, int Launch_dt, float Xi, float Yi);
    ~Path();
};

#endif 
