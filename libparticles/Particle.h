/*
 * particle.h - generic particle class
 * Release into public domain.
 */

#ifndef particle_h
#define particle_h
#define MAX_DIM 255

#include "Arduino.h"

class Particle
{
	public:
		byte x;   //left
		byte y;   //bottom
		signed char vx;  //horizontal velocity
		signed char vy;  //vertical velocity
		signed char ax;  //vertical acceleration
		signed char ay;  //horizontal acceleration
		byte ttl; //time to live
		byte hue; //hue
		boolean isAlive; //is alive?

		Particle();
		void update();
	private:
};

#endif /* particle_h */
