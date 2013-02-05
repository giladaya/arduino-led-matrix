#include <Colorduino.h>
#include "BounceParticle.h"
#include "SpinEmitter.h"
//#include "RainEmitter.h"
#include "ParticleSys.h"

//#define DEBUG 1

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

typedef struct
{
    signed char x;
    signed char y;
} VactorCart;

const byte res = 8;
const byte maxDim = 255;
const byte numParticles = 50;
const byte pWidth = maxDim / res + 1; //32
const int pSurface = pWidth*pWidth; //1024
unsigned int pCount = 0;
boolean pulseOn = false;

BounceParticle particles[numParticles];
SpinEmitter emitter(112, 112, 5, 2);
//RainEmitter emitter;
ParticleSys pSys(numParticles, particles, &emitter);

ColorRGB matrix[res][res];

void addColorToMatrix(byte col, byte row, void *vHSV){
    ColorHSV *colorHSV=(ColorHSV *)vHSV;
    ColorRGB colorRGB;
    int tempVal, res;
    
    if (colorHSV->v == 0){
        return;
    }
    
    HSVtoRGB(&colorRGB, colorHSV); 
    
    tempVal = matrix[col][row].r + colorRGB.r;
    matrix[col][row].r = min(tempVal, 255);
    if (tempVal > 255){
        res = tempVal-255;
        res = res>>1;
        matrix[col][row].g = min((matrix[col][row].g+res), 255);
        matrix[col][row].b = min((matrix[col][row].b+res), 255);
    }
    tempVal = matrix[col][row].g + colorRGB.g;
    matrix[col][row].g = min(tempVal, 255);
    if (tempVal > 255){
        res = tempVal-255;
        res = res>>1;
        matrix[col][row].r = min((matrix[col][row].r+res), 255);
        matrix[col][row].b = min((matrix[col][row].b+res), 255);
    }
    tempVal = matrix[col][row].b + colorRGB.b;
    matrix[col][row].b = min(tempVal, 255);
    if (tempVal > 255){
        res = tempVal-255;
        res = res>>1;
        matrix[col][row].r = min((matrix[col][row].r+res), 255);
        matrix[col][row].g = min((matrix[col][row].g+res), 255);
    }
}

void drawMatrix(){
    byte row, col, dx, dy;
    long tempVal;
    ColorHSV colorHSV;
    
    fadeMatrix();
    
    //go over particles and update matrix cells on the way
    for(int i = 0; i<numParticles; i++){
        //generate RGB values for particle
        colorHSV.h = particles[i].hue;
        colorHSV.s = 255;
        
        dx = pWidth - (particles[i].x % pWidth);
        dy = pWidth - (particles[i].y % pWidth);

        //bottom left
        col = particles[i].x / pWidth;
        row = particles[i].y / pWidth;
        tempVal = (long)dx*dy*particles[i].ttl/pSurface;
        colorHSV.v = (byte)tempVal;
        addColorToMatrix(col, row, &colorHSV);

        //bottom right;
        col++;
        if (col < res){
            tempVal = (long)(pWidth-dx)*dy*particles[i].ttl/pSurface;
            colorHSV.v = (byte)tempVal;
            addColorToMatrix(col, row, &colorHSV);
        }

        //top right
        row++;
        if (col < res && row < res){
            tempVal = (long)(pWidth-dx)*(pWidth-dy)*particles[i].ttl/pSurface;
            colorHSV.v = (byte)tempVal;
            addColorToMatrix(col, row, &colorHSV);
        }
        
        //top left
        col--;
        if (row < res){
            tempVal = (long)dx*(pWidth-dy)*particles[i].ttl/pSurface;
            colorHSV.v = (byte)tempVal;
            addColorToMatrix(col, row, &colorHSV);
        }

#ifdef DEBUG        
        if (i == 0) Serial.println("--------------");
#endif   
    }
    
    for (byte y=0;y<res;y++) {
        for(byte x=0;x<res;x++) {
            Colorduino.SetPixel(x, y, matrix[x][y].r, matrix[x][y].g, matrix[x][y].b);
        }
    }
}

void resetMatrix(){
    //init all pixels to zero
    for (byte y=0;y<ColorduinoScreenWidth;y++) {
        for(byte x=0;x<ColorduinoScreenHeight;x++) {
            matrix[x][y].r = 0;
            matrix[x][y].g = 0;
            matrix[x][y].b = 0;
        }
    }
}

void fadeMatrix(){
    //fade all pixels
    for (byte y=0;y<ColorduinoScreenWidth;y++) {
        for(byte x=0;x<ColorduinoScreenHeight;x++) {
            matrix[x][y].r = matrix[x][y].r>>1;
            matrix[x][y].g = matrix[x][y].g>>1;
            matrix[x][y].b = matrix[x][y].b>>1;
        }
    }
}

void setup()
{
#ifdef DEBUG    
  // start serial port at 9600 bps:
  Serial.begin(9600);
  Serial.print("start. width:");
  Serial.println(pWidth);
#endif  
  
  Colorduino.Init(); // initialize the board
  
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  byte whiteBalVal[3] = {36,63,7}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  randomSeed(analogRead(0));
 
  //init all pixels to zero
  resetMatrix();
 
  Colorduino.FlipPage(); // swap screen buffers to show it
}

void loop()
{
    pSys.update();
    drawMatrix();
//    if (pulseOn){
//        Colorduino.SetPixel(7, 7, 200, 0, 0);
//    }
//    else {
//        Colorduino.SetPixel(7, 7, 0, 0, 0);
//    }
//    pulseOn = ! pulseOn;
    
    Colorduino.FlipPage();
    delay(20);
}

/**
 * convert polar coordinates to cartesian vector
 */
//void polarToCart(byte r, word angle, void *vCart){
//    VactorCart *vectorCart=(VactorCart *)vCart;
//    float radAngle;
//    
//    // Conver from Degree -> Rad
//    radAngle = -angle*(PI/180) ;
//    // Convert Polar -> Cartesian
//    vectorCart->x = (signed char)(r * cos(radAngle));
//    vectorCart->y = (signed char)(r * sin(radAngle));
//}

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

