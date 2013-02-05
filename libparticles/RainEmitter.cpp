/*
 * RainEmitter.cpp - a rainning emitter
 * Release into public domain.
 */

#include "RainEmitter.h"

RainEmitter::RainEmitter(char side, byte maxDim)
{
    counter = 0;
    baseHue = 128; //blues
    this->side = side;
    this->maxDim = maxDim;
}

void RainEmitter::emit(Particle * particle)
{
   counter++;
   
   switch(side)
   {
      case 't':
          particle->x = random(maxDim);
          particle->y = maxDim-32;
          break;
      case 'r':
          particle->x = maxDim-32;
          particle->y = random(maxDim);
          break;
      case 'b':
          particle->x = random(maxDim);
          particle->y = 0;
          break;
      case 'l':
          particle->x = 0;
          particle->y = random(maxDim);
          break;
   }
    
   particle->vx = 0;
   particle->vy = 0;
    
   particle->ttl = random(16);
//   particle->hue = random(250);
   particle->hue = counter%250;
//   particle->hue = baseHue+random(16);
   particle->isAlive = true;
}
