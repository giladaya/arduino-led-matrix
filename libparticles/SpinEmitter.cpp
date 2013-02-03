/*
 * SpinEmitter.cpp - a spinning emitter
 * Release into public domain.
 */

#include "SpinEmitter.h"

SpinEmitter::SpinEmitter(byte x, byte y, byte r)
{
    this->x = x;
    this->y = y;
    this->r = r;
    counter = 0;
}

void SpinEmitter::emit(Particle * particle)
{
   float radAngle;
   
   counter++;
   particle->x = this->x;
   particle->y = this->y;
    
   // Conver from Degree -> Rad
   radAngle = -counter*10*(PI/180) ;
   // Convert Polar -> Cartesian
   particle->vx = (signed char)(r * cos(radAngle));
   particle->vy = (signed char)(r * sin(radAngle));
    
   particle->ax = 0;
   particle->ay = 0;
   particle->ttl = random(10,100);
   particle->hue = counter%255;
   particle->isAlive = true;
}
