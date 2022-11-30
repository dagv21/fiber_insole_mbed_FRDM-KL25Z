#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"
#include <cmath>

// Inputs declarations - Photodetectors
AnalogIn PD1(A0); // Photodetector 1
AnalogIn PD2(A2); // Photodetector 2

#define WS2812_BUF 9 // Ten LEDs

PixelArray px(WS2812_BUF);

// See the program page for information on the timing numbers
// The given numbers are for the K64F
WS2812 ws(PTE20, WS2812_BUF, 0, 5, 5, 0); //Timing Numbers from Datasheet

enum LED_COLORS {
    BLUE        = 0x0000FF,
    LIGHTBLUE   = 0x00FFF6,
    RED         = 0xFF0000,
    ORANGE      = 0xFF3500,
    GREEN       = 0X00FF00,
    BLACK       = 0X000000,
    WHITE       = 0XFFFFFF,
    PURPLE      = 0XFF00FF,
    PINK        = 0XFF84A3,
    YELLOW      = 0XFFFF00,
    DARK_YELLOW = 0X555500,
    DEFAULT     = 0x000000
};

int iterations_on = 20;  //Delay for LEDs
int iterations_off = 10;  //Delay for LEDs

// Variables to store reads from photodetectors
unsigned long PD1_S = 0;
unsigned long PD2_S = 0;
unsigned long PD1_S_vector[WS2812_BUF];
unsigned long PD2_S_vector[WS2812_BUF]; 
unsigned long PD_S_on = 0;
unsigned long PD_S_off = 0;

RawSerial pc(USBTX, USBRX);


int main()
{
    px.SetAll(BLACK);
    pc.baud(1000000);

    while (1) {
        for (int z=0; z <= WS2812_BUF ; z++) {
        //for (int z=0; z <= 0 ; z++) {
            ///// Turning off /////
            px.Set(z, BLACK); //Black color
            ws.write(px.getBuf()); //Turn off

            ///// Reading Photodetectors /////
            PD1_S = PD1.read_u16();
            PD2_S = PD2.read_u16();
            for(int i=0; i<iterations_off-1; i++){
                PD1_S = PD1_S + PD1.read_u16();
                PD2_S = PD2_S + PD2.read_u16();
            }
            PD1_S = PD1_S/iterations_off;
            PD2_S = PD2_S/iterations_off;
            PD_S_off = (PD1_S + PD2_S)/2;

            ///// Turning on /////
            px.Set(z, RED); //Light color
            ws.write(px.getBuf()); //Turn on

            ///// Reading Photodetectors /////
            PD1_S = PD1.read_u16();
            PD2_S = PD2.read_u16();
            for(int i=0; i<iterations_on-1; i++){
                PD1_S = PD1_S + PD1.read_u16();
                PD2_S = PD2_S + PD2.read_u16();
            }
            PD1_S_vector[z] = PD1_S/iterations_on;
            PD2_S_vector[z] = PD2_S/iterations_on;
            //PD_S_on = (PD1_S + PD2_S)/2;

            ///// Turning off /////
            px.Set(z, BLACK); //Black color
            ws.write(px.getBuf()); //Turn off

            //wait_ms(500); //Sleep in miliseconds
        }
        ///// Sending Data ///// 
        for(int i=0;i<WS2812_BUF-1;i++)
	        pc.printf("%lu\t",PD1_S_vector[i]);
        pc.printf("%lu \n",PD1_S_vector[WS2812_BUF]); 

    }

}
