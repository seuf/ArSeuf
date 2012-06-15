#ifndef __MOTOR_H__
#define __MOTOR_H__

class Motor {
  public :
    Motor();
	void attach();
	void goForward(int speed);
	void goBack(int speed);
	void stopRobot();
	void turnLeft(int speed);
	void turnRight(int speed);
	void stopTurn(int speed);
	
};

#endif
