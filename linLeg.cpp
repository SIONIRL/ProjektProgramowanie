#include "linLeg.h"

linLeg::linLeg(void) {}

linLeg::linLeg(float* pxi, float* pyi, float Vxi, float Vyi, float per )
	{
		posix = *pxi;
		posiy = *pyi;

		velix = Vxi;
		veliy = Vyi;

		period = per;

		*pxi = x(per);
		*pyi = y(per);

	}

linLeg::linLeg(float xi, float yi, float xf, float yf, float speed )
{
    posix = xi;
    posiy = yi;
    float dist = sqrt( (xf-xi)*(xf-xi) + (yf-yi)*(yf-yi) );
    period = dist/speed;
    velix = speed*(xf-xi)/dist;
    veliy = speed*(yf-yi)/dist;
}

void linLeg::INIT(float xi, float yi, float xf, float yf, float speed )
{
    posix = xi;
    posiy = yi;
    float dist = sqrt( (xf-xi)*(xf-xi) + (yf-yi)*(yf-yi) );
    period = dist/speed;
    velix = speed*(xf-xi)/dist;
    veliy = speed*(yf-yi)/dist;
    return;
}

linLeg::~linLeg(void)
{
}

float linLeg::x( float t )
	{
		return( posix + velix*t );
	}

float linLeg::y( float t )
	{
		return( posiy + veliy*t );
	}
