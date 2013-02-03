/*
 * particle.cpp - generic particle class
 * Release into public domain.
 */

#include "Particle.h"

Particle::Particle()
{
   isAlive = false;
}

void Particle::update(){
    //age
    ttl--;
    
    //apply acceleration
    vx = min(vx+ax, MAX_DIM);
    vy = min(vy+ay, MAX_DIM);
    
    //apply velocity
    unsigned int newX, newY;
    //~
    if (y == 0 || y == MAX_DIM){
        vy = -1*vy;
    }
    if (x == 0 || x == MAX_DIM){
        vx = -1*vx;
    }
    if (ttl == 0 || (vx == 0 && vy == 0)){
        isAlive = false;
    }
    else {
        x = min(max(x+vx, 0), MAX_DIM);
        y = min(max(y+vy, 0), MAX_DIM);
    }
    return;
    //~

//    newX = x + vx;
//    newY = y + vy;
//    if(ttl == 0 || newX < 0 || newX > MAX_DIM || newY < 0 || newY > MAX_DIM){
//        isAlive = false;
//    }
//    else {
//        x = (byte)newX;
//        y = (byte)newY;
//    }
 }

