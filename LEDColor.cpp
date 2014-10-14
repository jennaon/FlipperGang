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
