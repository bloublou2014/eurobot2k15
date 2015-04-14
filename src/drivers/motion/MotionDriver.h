/*
 * KretackiDrajver.h
 *
 *  Created on: Nov 7, 2013
 *      Author: LaptopX
 */

#ifndef KRETACKIDRAJVER_H_
#define KRETACKIDRAJVER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include "utils/uart/UartConnection.h"
#include "utils/geometry/Point2D.h"

namespace motion
{

class MotionDriver {
public:
	enum State{IDLE, STUCK, MOVING, ROTATING, ERROR};
	enum MovingDirection{FORWARD=1, BACKWARD=-1};
	enum RobotType {MALI = 1, VELIKI = -1};

private:
	geometry::Point2D position;
	int orientation;
	State state;
	char speed;
	MovingDirection direction;
	int reverseAngle;

    uart::UartConnection uart;

	int convertToInt(char msb, char lsb);
	
	boost::mutex *io_mutex;

public:
	MotionDriver(geometry::Point2D initPosition=geometry::Point2D(), RobotType robotType=VELIKI, int initOrientation=0, int initSpeed=100);
	~MotionDriver();
	
	void moveStraight(int distance);
	void rotateFor(int relativeAngle);
	void rotateTo(int absoluteAngle);
	void moveToPosition(geometry::Point2D position, MovingDirection direction);
	void moveArc(geometry::Point2D center, int angle, MovingDirection direction);
	void stop();
    void softStop();

	char getSpeed();
	geometry::Point2D getPosition();
	int getOrientation();
	State getState();
	int getDirection();

	void setSpeed(char speed);
	void setPositionAndOrientation(const geometry::Point2D position, int orientation=0);

	void refreshData();
};

}

#endif /* KRETACKIDRAJVER_H_ */
