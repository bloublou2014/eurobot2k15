/*
 * GeometryUtil.h
 *
 *  Created on: Oct 23, 2013
 *      Author: PCX
 */

#ifndef GEOMETRYUTIL_H_
#define GEOMETRYUTIL_H_

#include "Point2D.h"

namespace geometry {

class GeometryUtil {
public:
	static double perpDot(const Point2D &a, const Point2D &b);
	static int linesIntersect(const Point2D &A1, const Point2D &A2, const Point2D &B1, const Point2D &B2);
};

} /* namespace geometry */
#endif /* GEOMETRYUTIL_H_ */
