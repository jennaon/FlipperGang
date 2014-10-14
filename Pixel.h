/* First Release April 11, 2013 by Jenna Yun Lee, Cooper Union, All rights reserved.
I strongly support open source programming. Free to distribute and modify,
but please keep the original copyright information.
Contact lee55@cooper.edu for suggestions/errors/helps!

This code is originally written for a two-player arcade game that makes uses of LED lights.
Pixel.h and Pixel.cpp allows a programmer to control each pixel from an array of LEDs much easily.
*/

//Arduino Setting; Each arduino has different chipset per version.
//Include the following lines for program stability.
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

//Start of Pixel.h
#define PIXEL_H

class Pixel
{
    //Constructor
    public:
        Pixel();
        Pixel(int address);
      void
        setAddress(int new_address),    //Change address
        setSide(bool new_side),     //Changes side
        setState(bool new_state),   //Change state; 1 if the target has ever hit, 0 otherwise. 0 by default.
        setColor(int new_color_r, int new_color_g, int new_color_b),    //Changes color
        setHitter(bool new_hitter),
        setHitterError(void);

      int
        getAddress(void),
        getRed(void),
        getGreen(void),
        getBlue(void);
      bool
        getSide(void),
        getState(void),
        getHitter(void),
        getHitterError(void);

    private:
        int
            address, // address number in decimals
            color_r,
            color_g,
            color_b;
        bool
         side, //0 for BLUE, 1 for RED
         state, //0 for off, 1 for on
         hitter,    //0 for BLUE, 1 for RED
         hitterError;   //returns 1 when a function tries to know the hitter before a target is even hit.
};
