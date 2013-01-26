#include <Colorduino.h>

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct 
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

//const unsigned char valueMask[ColorduinoScreenWidth][ColorduinoScreenHeight]={
//    {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  },
//    {32 , 32 , 32 , 32 , 32 , 32 , 32 , 32 }, 
//    {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 },
//    {96 , 96 , 96 , 96 , 96 , 96 , 96 , 96 }, 
//    {128, 128, 128, 128, 128, 128, 128, 128},
//    {160, 160, 160, 160, 160, 160, 160, 160},
//    {192, 192, 192, 192, 192, 192, 192, 192}, 
//    {255, 255, 255, 255, 255, 255, 255, 255}, 
//};
const unsigned char valueMask[ColorduinoScreenWidth][ColorduinoScreenHeight]={
    {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
    {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
    {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
    {128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
    {160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
    {192, 128, 96 , 64 , 64 , 96 , 128, 192},
    {255, 160, 128, 96 , 96 , 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255}
};

const unsigned char hueMask[ColorduinoScreenWidth][ColorduinoScreenHeight]={
    {3, 5, 7, 9, 9, 8, 6, 4},
    {2, 5, 7, 7, 7, 7, 5, 3},
    {2, 4, 6, 6, 6, 6, 5, 2},
    {1, 4, 5, 5, 5, 5, 4, 2},
    {1, 2, 4, 4, 4, 4, 2, 1},
    {0, 1, 2, 3, 3, 2, 1, 0},
    {0, 0, 1, 2, 2, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
};

ColorHSV matrix[ColorduinoScreenWidth][ColorduinoScreenHeight];
unsigned char line[ColorduinoScreenWidth];
int pcnt = 0;
unsigned char gx = 0; //temporary

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

/**
 * Randomly generate the next line (row)
 */
void generateLine(){
  for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      line[x] = random(64, 255);
  }
}

/**
 * shift all values in the matrix up one row
 */
void shiftUp(){
  ColorRGB colorRGB;
  
  for (unsigned char y=ColorduinoScreenWidth-1;y>0;y--) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        matrix[x][y].v = matrix[x][y-1].v;
    }
  }
  
  for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      matrix[x][0].v = line[x];
  }
}

/**
 * draw a frame, interpolating between 2 "key frames"
 * @param pcnt percentage of interpolation
 */
void drawFrame(int pcnt){
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  int nextv;
  
  for (unsigned char y=ColorduinoScreenWidth-1;y>0;y--) {
    for (unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        colorHSV.h = (unsigned char)(((100.0-pcnt)*matrix[x][y].h + pcnt*matrix[x][y-1].h)/100.0);
        colorHSV.s = (unsigned char)(((100.0-pcnt)*matrix[x][y].s + pcnt*matrix[x][y-1].s)/100.0);
        nextv = 
            (unsigned char)(((100.0-pcnt)*matrix[x][y].v 
          + pcnt*matrix[x][y-1].v)/100.0) 
          - valueMask[y][x];
        colorHSV.v = (unsigned char)max(0, nextv);
        
        HSVtoRGB(&colorRGB, &colorHSV);
        Colorduino.SetPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
    }
  }
  
  for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
//      colorHSV.h = (unsigned char)(((100.0-pcnt)*matrix[x][0].h + pcnt*hueMask[0][x])/100.0);
      colorHSV.h = hueMask[0][x];
      colorHSV.s = 255;
      colorHSV.v = (unsigned char)(((100.0-pcnt)*matrix[x][0].v + pcnt*line[x])/100.0);
      
      HSVtoRGB(&colorRGB, &colorHSV);
      Colorduino.SetPixel(x, 0, colorRGB.r, colorRGB.g, colorRGB.b);
  }
  Colorduino.FlipPage(); // swap screen buffers to show it
}

void setup()
{
  Colorduino.Init(); // initialize the board
  
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  unsigned char whiteBalVal[3] = {36,63,7}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  randomSeed(analogRead(0));
  generateLine();
  
  //init all pixels to zero
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        matrix[x][y].h = hueMask[y][x];
        matrix[x][y].s = 255;
        matrix[x][y].v = 0;
        //Colorduino.SetPixel(x, y, 0, 0, 0);
    }
  }
  
  Colorduino.FlipPage(); // swap screen buffers to show it
}

void loop()
{
    if (pcnt >= 100){
        shiftUp();
        generateLine();
        pcnt = 0;
    }
    drawFrame(pcnt);
    pcnt+=20;
//    delay(50);
    //fillWhite();
}
