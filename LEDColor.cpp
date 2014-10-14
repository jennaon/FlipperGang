/* First Release April 4, 2013 by Jenna Yun Lee, Cooper Union, All rights reserved.
I strongly support open source programming. Free to distribute and modify,
but please keep the original copyright information.
Contact yunjung.jenna.lee@gmail.com for suggestions/errors/helps!

This code is originally written for a two-player arcade game that makes uses of LED lights.
Pixel.h and Pixel.cpp allows a programmer to control each pixel from an array of LEDs much easily.
*/

#include "LedColor.h"

LedColor::LedColor()
{
}

LedColor::LedColor(int color)
{
    red = color;
    green = color;
    blue = color;
}

LedColor::LedColor(int new_red, int new_green, int new_blue)
{
    red = new_red;
    green = new_green;
    blue = new_blue;
}

int LedColor::getRed(void)
{
    return red;
}

int LedColor::getGreen(void)
{
    return green;
}

int LedColor::getBlue(void)
{
    return blue;
}
