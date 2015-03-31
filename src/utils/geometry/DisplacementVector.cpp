
#include "DisplacementVector.h"
#include "math.h"

namespace geometry {

DisplacementVector::DisplacementVector(const int &orientation, const int &speed)
{
	// racuna vektor pomeraja po jedinici vremena
    double orientationRad = orientation * (M_PI/180);
    dx = cos(orientationRad)*speed;
    dy = sin(orientationRad)*speed;
}

double DisplacementVector::getDx() const
{
    return dx;
}

double DisplacementVector::getDy() const
{
    return dy;
}

}
