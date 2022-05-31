#ifndef LINLEG_H
#define LINLEG_H

#include "Leg.h"
#include<cmath>


class linLeg : public Leg
{
    public:

    float posix;
	float posiy;
	float velix;
	float veliy;

	
	virtual float x( float t );
	virtual float y( float t );


	void INIT(float xi, float yi, float xf, float yf, float speed );

	linLeg(void);
	linLeg(float* pxi, float* pyi, float Vxi, float Vyi, float per );
	linLeg(float xi, float yi, float xf, float yf, float speed );
	virtual ~linLeg(void);
};

#endif
