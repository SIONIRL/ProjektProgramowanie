#ifndef LEG_H
#define LEG_H


class Leg
{
    public:

    float period;

	virtual float x( float t ) = 0;
	virtual float y( float t ) = 0;

	virtual void INIT(float xi, float yi, float xf, float yf, float speed ) = 0;

    Leg();
    virtual ~Leg();
};

#endif
