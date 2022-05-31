#include "Path.h"

sf::RenderWindow* Path::pWndw = NULL;

Path::Path(int N_legs, int Delay, int Launch_dt, float Xi, float Yi)
{
    nLegs = nLegs_inUse = N_legs;

    if( nLegs > 1 )
    {
        ppSprite = new sf::Sprite*[nLegs];
        ppLeg = new Leg*[nLegs];
        px = new float[nLegs];
        py = new float[nLegs];
        ptLeg = new float[nLegs];
    }
    else
    {
        ppSprite = new sf::Sprite*;
        ppLeg = new Leg*;
        px = new float;
        py = new float;
        ptLeg = new float;
    }

    xi = Xi;
    yi = Yi;
    delay = Delay;
    launch_dt = Launch_dt;
    inUse = done = false;

    for(int i=0; i<nLegs; ++i)
    {
        ppLeg[i] = NULL;
        ppSprite[i] = NULL;
        ptLeg[i] = static_cast<float>( -launch_dt*i );
        px[i] = Xi;
        py[i] = Yi;
    }
}

Path::~Path()
{
    if( nLegs > 1 )
    {
        for(int i=0; i<nLegs; ++i)
        {
        
            if( ppLeg[i] ) delete ppLeg[i];
        }
        delete [] ppSprite;
        delete [] ppLeg;
        delete [] px;
        delete [] py;
        delete [] ptLeg;
    }
    else
    {
        delete ppSprite;
        if( *ppLeg ) delete *ppLeg;
        delete ppLeg;
        delete px;
        delete py;
        delete ptLeg;
    }
}

void Path::reset( bool InUse, int Delay )
{
    inUse = InUse;
    done = false;
    delay = Delay;
    for(int i=0; i<nLegs; ++i)
    {
        ptLeg[i] = static_cast<float>( -launch_dt*i );
        px[i] = ppLeg[i]->x(0.0f);
        py[i] = ppLeg[i]->y(0.0f);
    }

    return;
}

void Path::move()
{
    if( inUse )
    {
        if( delay > 0 )
        {
            --delay;
            if( delay > 0 ) return;
        }

        inUse = false;
        for(int i=0; i<nLegs_inUse; ++i)
        {
            if( ptLeg[i] < ppLeg[i]->period )
            {
                ++ptLeg[i];
                inUse = true;
                if( ptLeg[i] > ppLeg[i]->period )
                    ptLeg[i] = ppLeg[i]->period;
                if( ptLeg[i] >= 0.0f )
                {
                    px[i] = ppLeg[i]->x( ptLeg[i] );
                    py[i] = ppLeg[i]->y( ptLeg[i] );
                }
            }
        }
    }

    return;
}

void Path::draw()
{

        for(int i=0; i<nLegs_inUse; ++i)
            if( ptLeg[i] >= 0.0f )
            {
                ppSprite[i]->SetPosition( px[i], py[i] );
                pWndw->Draw( *ppSprite[i] );
            }

    return;
}
