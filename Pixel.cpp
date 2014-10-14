/* First Release April 11, 2013 by Jenna Yun Lee, Cooper Union, All rights reserved.
I strongly support open source programming. Free to distribute and modify,
but please keep the original copyright information.
Contact lee55@cooper.edu for suggestions/errors/helps!

This code is originally written for a two-player arcade game that makes uses of LED lights.
Pixel.h and Pixel.cpp allows a programmer to control each pixel from an array of LEDs much easily.
*/

#include "Pixel.h"
//Constructors
Pixel::Pixel() {}

Pixel::Pixel(int addr)
{
    address = addr;
    state = 0;
    //Our arcade game uses 5x5 LED grid, so 50 flippers for two sides.
    //Side 0 if address number is less than 24 (up to 25th LED), Side 1 otherwise
    if (addr>24) side = 1;
    else side =0;
}

//A series of getter-setter functions from here:

//Address
int Pixel::getAddress(void) {
    return address;
}
void Pixel::setAddress(int new_address) {
    address = new_address;
}

//Side
bool Pixel::getSide(void) {
    return side;
}
void Pixel::setSide(bool new_side){
    side = new_side;
}

//State
bool Pixel::getState(void){
     return state;
}
void Pixel::setState(bool new_state){
      state = new_state;
}

//Hitter
bool Pixel::getHitter(void){
     return hitter;
}
void Pixel::setHitter(bool new_hitter){
      hitter = new_hitter;
      setHitterError();
}

bool Pixel::getHitterError()
{
    return hitterError;
}
void Pixel::setHitterError()
{
    hitterError = 1;
}
//Color, RGB separately
int Pixel::getRed(void) {
    return color_r;
}
int Pixel::getGreen(void) {
    return color_g;
}
int Pixel::getBlue(void) {
    return color_b;
}
void Pixel::setColor(int new_color_r, int new_color_g, int new_color_b) {
    color_r = new_color_r;
    color_g = new_color_g;
    color_b = new_color_b;
}
