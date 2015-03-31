
namespace Geometry {


class Point2D {
private:
    int x;
    int y;
public:
    Point2D();
    Point2D(int x, int y);
    bool operator==(const Point2D &p) const;
    Point2D& operator=(const Point2D &rhs);
    int euclidDist(const Point2D &p) const;
    int relativeAngleTo(const Point2D &p) const;
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
};

class GeometryUtil{
public:
    static double perpDot(const Point2D &a, const Point2D &b);
    static int linesIntersect(const Point2D &A1, const Point2D &A2, const Point2D &B1, const Point2D &B2);
};

class DisplacementVector
{
    double dx, dy;

public:
    DisplacementVector(const int &orientation, const int &speed);
    double getDx() const;
    double getDy() const;
};

}
