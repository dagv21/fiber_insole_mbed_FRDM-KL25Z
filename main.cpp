#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"

#define WS2812_BUF 60

PixelArray px(WS2812_BUF);

// See the program page for information on the timing numbers (eg: 0, 5, 5, 0)
// The given numbers are for the K64F
WS2812 ws(D9,WS2812_BUF, 0, 5, 5, 0);

DigitalOut led(LED1);

int main()
{

    ws.useII(WS2812::PER_PIXEL); // use per-pixel intensity scaling
    
    // set up the colours we want to draw with
    int colorbuf[6] = {0x2f0000,0x2f2f00,0x002f00,0x002f2f,0x00002f,0x2f002f};

    // for each of the colours (j) write out 10 of them
    // the pixels are written at the colour*10, plus the colour position
    // all modulus 60 so it wraps around
    for (int i =0; i<6; i++) {
        for (int j=0; j<10; j++) {
            px.Set(((i*10)+j)%60,colorbuf[i]);
        }
    }

    // now all the colours are computed, add a fade effect using intensity scaling
    // compute and write the II value for each pixel
    for (int j=0; j<60; j++) {
        // px.SetI(pixel position, II value)
        px.SetI(j%60, 0xf+(0xf*(j%10)));
    }


    // Now the buffer is written, rotate it
    // by writing it out with an increasing offset
    while (1) {
        for (int z=59; z >= 0 ; z--) {
            ws.write_offsets(px.getBuf(),z,z,z);
            wait(0.075);
        }
    }

}
