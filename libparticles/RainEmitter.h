/*
 * RainEmitter.h - a rainning emitter
 * Release into public domain.
 */

#ifndef rain_emitter_h
#define rain_emitter_h

#include "Emitter.h"
#include "Arduino.h"

class RainEmitter : public Emitter
{
    public:
        unsigned int counter;
        byte baseHue;
        
        RainEmitter();
        void emit(Particle * particle);
    private:
};

#endif /* rain_emitter_h */
