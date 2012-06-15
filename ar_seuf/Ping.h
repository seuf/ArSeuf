#ifndef __PING_H__
#define __PING_H__


class Ping {
  public : 
	Ping();
	
	/**
	* Ping
	* return the distance in centimeters for the clostest object
	*/
	int ping();
	
	
  private :
	/**
	* Convert sound ping duration into centimeters
	*/
	long microsecondsToCentimeters(long microseconds);
};

#endif
