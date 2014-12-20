
#ifndef DISPLACEMENT_VECTOR
#define DISPLACEMENT_VECTOR

namespace geometry {

// displacement vector per unit of time
class DisplacementVector
{
    double dx, dy;

public:
    DisplacementVector(const int &orientation, const int &speed);
    double getDx() const;
    double getDy() const;
};

} /* namespace geometry */

#endif
