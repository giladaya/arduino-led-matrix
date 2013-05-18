/* 
 * Copyright (C) 2013 Gilad Dayagi.  All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

 /*
  * An example for the Arduino particle system library
  * Creates a green flame effect.
  */
  
#define DEBUG 1;
//'r' for rainbowduino, 'c' for colorduino
#define BOARD 'r'

#if BOARD == 'c'
#include <Colorduino.h>
#else
#include <Rainbowduino.h>
#endif

#define M_WIDTH 8
#define M_HEIGHT 8

typedef struct
{
  byte r;
  byte g;
  byte b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct 
{
  byte h;
  byte s;
  byte v;
} ColorHSV;
  
unsigned int counter;

void setup()
{
#if BOARD == 'c'
    Colorduino.Init(); // initialize the board

    // compensate for relative intensity differences in R/G/B brightness
    // array of 6-bit base values for RGB (0~63)
    // whiteBalVal[0]=red
    // whiteBalVal[1]=green
    // whiteBalVal[2]=blue
    unsigned char whiteBalVal[3] = {
        36,63,6    }; // for LEDSEE 6x6cm round matrix
    Colorduino.SetWhiteBal(whiteBalVal);
#else
    Rb.init();
#endif
    
    counter = 0;

#ifdef DEBUG    
    Serial.begin(9600);
#endif    
}

void loop()
{
    ColorRGB colorRGB;
    ColorHSV colorHSV;

    colorHSV.v = 255;
    colorHSV.s = 255;
    colorHSV.h = counter%255;
    
    HSVtoRGB(&colorRGB, &colorHSV);
    uint32_t result = ((((uint32_t)colorRGB.r)<<16) | (((uint32_t)colorRGB.g)<<8) | colorRGB.b);
    
#ifdef DEBUG    
    Serial.print("H: ");
    Serial.print(colorHSV.h);
    Serial.print(" -> R: ");
    Serial.print(colorRGB.r);    
    Serial.print(" G: ");
    Serial.print(colorRGB.g);    
    Serial.print(" B: ");
    Serial.print(colorRGB.b);
    Serial.print(" -> ");
    Serial.println(result, HEX);
#endif    
    
#if BOARD == 'c'    
    Colorduino.ColorFill(colorRGB.r, colorRGB.g, colorRGB.b);
#else
    Rb.fillRectangle(0, 0, 8, 8, result);
#endif
    counter++;
    delay(100);
}

//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV) 
{
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  h = (float)(colorHSV->h / 256.0);
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //if saturation > 0, more complex calculations are needed
  else
  {
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = (int)(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);
}
