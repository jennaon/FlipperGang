/* First Release April 4, 2013 by Jenna Yun Lee, Cooper Union, All rights reserved.
I strongly support open source programming. Free to distribute and modify,
but please keep the original copyright information.
Contact yunjung.jenna.lee@gmail.com for suggestions/errors/helps!

This code is originally written for a two-player arcade game that makes uses of LED lights.
Pixel.h and Pixel.cpp allows a programmer to control each pixel from an array of LEDs much easily.
*/
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

#define LEDCOLOR_H


class LedColor
{
    public:
        LedColor();
        LedColor(int color);
        LedColor(int new_red, int new_green, int new_blue);
        int
            getRed(void),
            getBlue(void),
            getGreen(void);

    private:
        int
            red,
            green,
            blue;
};
