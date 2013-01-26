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

const char DECAY = 32; //vertical decay of luminosity
ColorHSV matrix[ColorduinoScreenWidth][ColorduinoScreenHeight];
ColorHSV line[ColorduinoScreenWidth];
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
  for(unsigned char x=0;x<ColorduinoScreenHeight;x+=2) {
      line[x].h = random(255);
      line[x].s = 255;//random(128, 255);
      line[x].v = random(64, 255);
      //if (x == gx) line[x].v = 0;
      //else line[x].v = 255;
  }
  for(unsigned char x=1;x<ColorduinoScreenHeight-1;x+=2) {
      line[x].h = (unsigned char)(0.5*line[x-1].h+0.5*line[x+1].h);
      line[x].s = (unsigned char)(0.5*line[x-1].s+0.5*line[x+1].s);
      line[x].v = (unsigned char)(0.5*line[x-1].v+0.5*line[x+1].v);
  }
  //last row
  line[ColorduinoScreenHeight-1].h = line[ColorduinoScreenHeight-2].h + random(50,100) - 50;
  line[ColorduinoScreenHeight-1].s = line[ColorduinoScreenHeight-2].s + random(50,100) - 50;
  line[ColorduinoScreenHeight-1].v = line[ColorduinoScreenHeight-2].v + random(50,100) - 50;
  gx++;
  if (gx == ColorduinoScreenHeight) gx = 0;
}

/**
 * Randomly generate the next line (row)
 */
void generateLine2(){
  int val = 0;
  
  //first pixel
  val = (int)(0.5*(line[0].h + line[1].h)) + random(64) - 32;
  line[0].h = (unsigned char)min(val, 255);
  line[0].v = random(64, 255);
  
  //last pixel
  val = (int)(0.5*(line[ColorduinoScreenHeight-2].h + line[ColorduinoScreenHeight-1].h)) + random(64) - 32;
  line[ColorduinoScreenHeight-1].h = (unsigned char)min(val, 255);
  line[ColorduinoScreenHeight-1].v = random(64, 255);
  
  for(unsigned char x=1;x<ColorduinoScreenHeight-1;x++) {
      val = (int)(0.33*(line[x-1].h + line[x].h + line[x+1].h)) + random(64) - 32;
      line[x].h = (unsigned char)min(val, 255);
      line[x].s = 255;//random(128, 255);
      //val = line[x].v + random(128) - 64;
      //line[x].v = max(64, min(val, 255));
      line[x].v = random(64, 255);
  }
}

/**
 * Create the first line of random pixels
 */
void initline(){
    line[0].h = random(255);
    line[0].s = 255;
    line[0].v = random(64, 255);
    
    int val = 0;
    for(unsigned char x=1;x<ColorduinoScreenHeight;x++) {
        val = line[x-1].h + random(128) - 64;
        line[x].h = (unsigned char)min(val, 255);
        line[x].s = 255;//random(128, 255);
        //val = line[x].v + random(128) - 64;
        //line[x].v = max(64, min(val, 255));
        line[x].v = random(64, 255);
    }
}

/**
 * shift all values in the matrix up one row
 */
void shiftUp(){
  ColorRGB colorRGB;
  
  for (unsigned char y=ColorduinoScreenWidth-1;y>0;y--) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        matrix[x][y].h = matrix[x][y-1].h;
        matrix[x][y].s = matrix[x][y-1].s;
        if(matrix[x][y-1].v < DECAY) { 
            matrix[x][y].v = 0;
        } else {
            matrix[x][y].v = matrix[x][y-1].v - DECAY;
        }
    }
  }
  
  for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      matrix[x][0].h = line[x].h;
      matrix[x][0].s = line[x].s;
      matrix[x][0].v = line[x].v;      
  }
}

/**
 * Fill all pixels with white - good for white balancing
 */
void fillWhite(){
    PixelRGB *p = Colorduino.GetPixel(0,0);
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
          p->r = 255;
          p->g = 255;
          p->b = 255;
          p++;
        }
    }
    Colorduino.FlipPage();
}

/**
 * draw a frame, interpolating between 2 "key frames"
 * @param pcnt percentage of interpolation
 */
void drawFrame(int pcnt){
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  unsigned char nextv;
  
  for (unsigned char y=ColorduinoScreenWidth-1;y>0;y--) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        colorHSV.h = (unsigned char)((100.0-pcnt)/100.0*matrix[x][y].h + pcnt*matrix[x][y-1].h/100.0);
        colorHSV.s = (unsigned char)((100.0-pcnt)/100.0*matrix[x][y].s + pcnt*matrix[x][y-1].s/100.0);
        
        if(matrix[x][y-1].v < DECAY) { 
            nextv = 0;
        } else {
            nextv = matrix[x][y-1].v - DECAY;
        }
        colorHSV.v = (unsigned char)((100.0-pcnt)/100.0*matrix[x][y].v + pcnt*(nextv)/100.0);
        HSVtoRGB(&colorRGB, &colorHSV);
        Colorduino.SetPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
    }
  }
  
  for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      colorHSV.h = (unsigned char)((100.0-pcnt)/100.0*matrix[x][0].h + pcnt*line[x].h/100.0);
      colorHSV.s = (unsigned char)((100.0-pcnt)/100.0*matrix[x][0].s + pcnt*line[x].s/100.0);
      colorHSV.v = (unsigned char)((100.0-pcnt)/100.0*matrix[x][0].v + pcnt*line[x].v/100.0);
      
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
  initline();
//  generateLine();
  
  //init all pixels to zero
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
        matrix[x][y].h = 0;
        matrix[x][y].s = 0;
        matrix[x][y].v = 0;
        Colorduino.SetPixel(x, y, 0, 0, 0);
    }
  }
  
  Colorduino.FlipPage(); // swap screen buffers to show it
}

void loop()
{
    if (pcnt >= 100){
        shiftUp();
        generateLine2();
        pcnt = 0;
    }
    drawFrame(pcnt);
    pcnt+=20;
//    delay(50);
    //fillWhite();
}
