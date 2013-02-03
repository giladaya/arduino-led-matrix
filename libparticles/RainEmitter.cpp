/*
 * RainEmitter.cpp - a rainning emitter
 * Release into public domain.
 */

#include "RainEmitter.h"

RainEmitter::RainEmitter()
{
    counter = 0;
    baseHue = 128; //blues
}

void RainEmitter::emit(Particle * particle)
{
   counter++;
   particle->x = random(MAX_DIM);
   particle->y = MAX_DIM-32;
    
   particle->vx = 0;
   particle->vy = 0;
    
   particle->ax = 0;
   particle->ay = -2;
   particle->ttl = random(16);
//   particle->hue = random(250);
   particle->hue = counter%250;
//   particle->hue = baseHue+random(16);
   particle->isAlive = true;
}
