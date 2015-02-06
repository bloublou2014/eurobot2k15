
#include "math.h"
#include "Geometry.h"

namespace Geometry {



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

//#######################
// ### GEOMETRY UTIL ###
//#######################


double GeometryUtil::perpDot(const Point2D &a, const Point2D &b)
{
    return (a.getY() * b.getX()) - (a.getX() * b.getY());
}

// da li se seku duzi
int GeometryUtil::linesIntersect(const Point2D &A1, const Point2D &A2, const Point2D &B1, const Point2D &B2)
{
    Point2D a;
    Point2D b;
    a.setX(A2.getX() - A1.getX());
    a.setY(A2.getY() - A1.getY());
    b.setX(B2.getX() - B1.getX());
    b.setY(B2.getY() - B1.getY());

    double f = GeometryUtil::perpDot(a, b);
    if (f == 0)      // lines are parallel
    {
        return false;
    }

    Point2D c;
    c.setX(B2.getX() - A2.getX());
    c.setY(B2.getY() - A2.getY());

    double aa = GeometryUtil::perpDot(a, c);
    double bb = GeometryUtil::perpDot(b, c);

    if (f < 0)
    {
        if (aa > 0) return false;
        if (bb > 0) return false;
        if (aa < f) return false;
        if (bb < f) return false;
    }
    else
    {
        if (aa < 0) return false;
        if (bb < 0) return false;
        if (aa > f) return false;
        if (bb > f) return false;
    }

    return true;
}

//#######################
// ### POINT 2D  ###
//#######################

Point2D::Point2D():x(0),y(0) {}
Point2D::Point2D(int x, int y):x(x),y(y){}

int Point2D::euclidDist(const Point2D &p) const
{
    return (int)sqrt(((double)(x - p.x) * (double)(x - p.x)) + ((double)(y - p.y) * (double)(y - p.y)));
}

int Point2D::relativeAngleTo(const Point2D &p) const
{
    return (int)((atan2((signed int)(p.y-y), (signed int)(p.x-x)) * 180.0) / M_PI);
}

bool Point2D::operator==(const Point2D &p) const
{
    return x==p.x && y==p.y;
}

int Point2D::getX() const
{
    return x;
}
int Point2D::getY() const
{
    return y;
}

void Point2D::setX(int x)
{
    this->x = x;
}

void Point2D::setY(int y)
{
    this->y = y;
}

Point2D& Point2D::operator=(const Point2D &rhs)
{
    if(this == &rhs) return *this;
    x = rhs.getX();
    y = rhs.getY();
    return *this;
}



}
