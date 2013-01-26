#include <Colorduino.h>

/**
 * red-green gradient
 * @param maxi maximum color intensity
 */
void gradRg(unsigned char maxi){
    int mult = maxi / ColorduinoScreenWidth;
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
            Colorduino.SetPixel(x, y, mult*x, mult*y, 0);
        }
    }
  
    Colorduino.FlipPage(); // swap screen buffers to show it
}

/**
 * red-blue gradient
 * @param maxi maximum color intensity
 */
void gradRb(unsigned char maxi){
    int mult = maxi / ColorduinoScreenWidth;
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
            Colorduino.SetPixel(x, y, x*mult, 0, y*mult);
        }
    }
  
    Colorduino.FlipPage(); // swap screen buffers to show it
}

/**
 * green-blue gradient
 * @param maxi maximum color intensity
 */
void gradGb(unsigned char maxi){
    int mult = maxi / ColorduinoScreenWidth;
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
            Colorduino.SetPixel(x, y, 0, x*mult, y*mult);
        }
    }
  
    Colorduino.FlipPage(); // swap screen buffers to show it
}

/**
 * Gradient fill
 * @param R maximum red intensity
 * @param G maximum green intensity
 * @param B maximum blue intensity
 */
void gradFill(unsigned char R,unsigned char G,unsigned char B){
    int multR = R / ColorduinoScreenWidth;
    int multG = G / ColorduinoScreenWidth;
    int multB = B / ColorduinoScreenWidth;
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
            Colorduino.SetPixel(x, y, x*multR, x*multG, x*multB);
        }
    }
  
    Colorduino.FlipPage(); // swap screen buffers to show it
}

/**
 * Solid color stripes:
 * red, yellow, green, cyan, blue, magenta, white, black
 * @param maxi max intensity for each color
 * @param shift shift of series (which pixel red would be in)
 */
void stripes(unsigned char maxi, unsigned char shift){
    for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
        Colorduino.SetPixel((shift)%ColorduinoScreenWidth, y, maxi, 0, 0); //red
        Colorduino.SetPixel((shift+1)%ColorduinoScreenWidth, y, maxi, maxi, 0); //yellow
        Colorduino.SetPixel((shift+2)%ColorduinoScreenWidth, y, 0, maxi, 0); //green
        Colorduino.SetPixel((shift+3)%ColorduinoScreenWidth, y, 0, maxi, maxi); //cyan
        Colorduino.SetPixel((shift+4)%ColorduinoScreenWidth, y, 0, 0, maxi); //blue
        Colorduino.SetPixel((shift+5)%ColorduinoScreenWidth, y, maxi, 0, maxi); //magenta
        Colorduino.SetPixel((shift+6)%ColorduinoScreenWidth, y, maxi, maxi, maxi); //white
        Colorduino.SetPixel((shift+7)%ColorduinoScreenWidth, y, 0, 0, 0); //black
    }
  
    Colorduino.FlipPage(); // swap screen buffers to show it
}

/**
 * Fill the entire matrix with solid color
 * @param R red value
 * @param G green value
 * @param B blue value
 */
void ColorFill(unsigned char R,unsigned char G,unsigned char B)
{
  PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
    }
  }
  
  Colorduino.FlipPage();
}

void setup()
{
  Colorduino.Init(); // initialize the board
  
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  unsigned char whiteBalVal[3] = {36,63,6}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  randomSeed(analogRead(0));
}

char cnt = 1;
void loop()
{
  switch (cnt) {
  case 1:
    //red-green gradient
    gradRg(192);
    break;
  case 2:
    //red-blue gradient
    gradRb(192);
    break;
  case 3:
    //green-blue gradient
    gradGb(192);
    break;
  case 4:
    //color stripes
    stripes(192, 0);
    break;
  case 5:
    //red gradient
    gradFill(192,0,0);
    break;
  case 6:
    //green gradient
    gradFill(0,192,0);
    break;
  case 7:
    //blue gradient
    gradFill(0,0,200);
    break;
  case 8:
    //gray gradient
    gradFill(192,192,192);
    break;
  default:
    cnt = 0;
    break;
  } 
  delay(5000);
  cnt++;
}
