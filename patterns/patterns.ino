//'r' for rainbowduino, 'c' for colorduino
#define BOARD 'r'

#if BOARD == 'c'
#include <Colorduino.h>
#else
#include <Rainbowduino.h>
#endif

#define M_WIDTH 8
#define M_HEIGHT 8

void setPixel(unsigned char x, unsigned char y, unsigned char colorR, unsigned char colorG, unsigned char colorB){
#if BOARD == 'c'
    Colorduino.SetPixel(x, y, colorR, colorG, colorB);
#else
    Rb.setPixelXY(x, y, colorR, colorG, colorB);
#endif
}

/**
 * red-green gradient
 * @param maxi maximum color intensity
 */
void gradRg(unsigned char maxi){
    int mult = maxi / M_WIDTH;
    for (unsigned char y=0;y<M_WIDTH;y++) {
        for(unsigned char x=0;x<M_HEIGHT;x++) {
            setPixel(x, y, mult*x, mult*y, 0);
        }
    }

    
}

/**
 * red-blue gradient
 * @param maxi maximum color intensity
 */
void gradRb(unsigned char maxi){
    int mult = maxi / M_WIDTH;
    for (unsigned char y=0;y<M_WIDTH;y++) {
        for(unsigned char x=0;x<M_HEIGHT;x++) {
            setPixel(x, y, x*mult, 0, y*mult);
        }
    }
}

/**
 * green-blue gradient
 * @param maxi maximum color intensity
 */
void gradGb(unsigned char maxi){
    int mult = maxi / M_WIDTH;
    for (unsigned char y=0;y<M_WIDTH;y++) {
        for(unsigned char x=0;x<M_HEIGHT;x++) {
            setPixel(x, y, 0, x*mult, y*mult);
        }
    }
}

/**
 * Gradient fill
 * @param R maximum red intensity
 * @param G maximum green intensity
 * @param B maximum blue intensity
 */
void gradFill(unsigned char R,unsigned char G,unsigned char B){
    int multR = R / M_WIDTH;
    int multG = G / M_WIDTH;
    int multB = B / M_WIDTH;
    for (unsigned char y=0;y<M_WIDTH;y++) {
        for(unsigned char x=0;x<M_HEIGHT;x++) {
            setPixel(x, y, x*multR, x*multG, x*multB);
        }
    }
}

/**
 * Solid color stripes:
 * red, yellow, green, cyan, blue, magenta, white, black
 * @param maxi max intensity for each color
 * @param shift shift of series (which pixel red would be in)
 */
void stripes(unsigned char maxi, unsigned char shift){
    for (unsigned char y=0;y<M_WIDTH;y++) {
        setPixel((shift)%M_WIDTH, y, maxi, 0, 0); //red
        setPixel((shift+1)%M_WIDTH, y, maxi, maxi, 0); //yellow
        setPixel((shift+2)%M_WIDTH, y, 0, maxi, 0); //green
        setPixel((shift+3)%M_WIDTH, y, 0, maxi, maxi); //cyan
        setPixel((shift+4)%M_WIDTH, y, 0, 0, maxi); //blue
        setPixel((shift+5)%M_WIDTH, y, maxi, 0, maxi); //magenta
        setPixel((shift+6)%M_WIDTH, y, maxi, maxi, maxi); //white
        setPixel((shift+7)%M_WIDTH, y, 0, 0, 0); //black
    }
}

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
#if BOARD == 'c'    
    Colorduino.FlipPage(); // swap screen buffers to show it
#endif    
    delay(5000);
    cnt++;
}

