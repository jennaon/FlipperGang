#include "SPI.h"
#include "Adafruit_WS2801.h"
#include "Pixel.h"
#include "Metro.h"
#include "LedColor.h"

uint8_t dataPin = 2;
uint8_t clockPin = 3;

int pinZ = A5; //PinZ set as Analog as we had a power inconsistence problem. 
int pinS = 7; //Start button
int pinA = 8;
int pinB = 9;
int pinC = 10;
int pinD = 11;
int pinE = 12; 
int pinF = 13;
//int pinSound = 22; // pin that deals with sound

int binary[6];
Pixel pixels[50];
int totnum = sizeof(pixels)/sizeof(Pixel);
boolean done = 0;
int ledCount = 0;
int score_0, score_1 = 0; //score_1 is red, score_0 is blue

Adafruit_WS2801 strip ((uint16_t)5, (uint16_t)5, dataPin, clockPin);

Metro m = Metro(3000000);    //3000 sec or 5 mins
LedColor neutral = LedColor(255);
LedColor draw = LedColor(20, 00, 40);
LedColor color_1(255, 0, 0); //red
LedColor color_0(0, 0, 255); //blue


void setup()
{
  pixelReset();  
  Serial.begin(9600);

  pinMode(pinZ, OUTPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, LOW);
  digitalWrite(pinZ, LOW);

  strip.begin();
  strip.show();
}

void loop()
{
  idle();
  startingSequence();
  pixelReset();
  ledReset();
  Serial.println("ROUND 2");
  ledCount = 0;
  delay(500);
  ledReset();
  pixelReset();

  //Random Number Generators
  LedColor c = getRandomColor();  //change to Color Wheel?
  int ledCountMax = 0;
  randomSeed(analogRead(0));
  int die = (int) random(3892);
  int helper = (int)random (12381);

  for(int z = 0; z<5; z++)
  {      
    die++;
    Serial.print("Round 2 Game #");
    Serial.println(z);
    if(z >2) die = helper;
    if(z>5) die = die+=helper;
    if(z>8) {
      randomSeed(analogRead(1));
      die = random(291);
    }
    c = getRandomColor();

    switch(die%3){
    case 1:    //drawLine
      {
        randomSeed(analogRead(0));
        long rand = random(100);
        int tempaddr = (int) random(0,int(rand));

        randomSeed(analogRead(0));
        rand = random(872);
        int count = (int) random(0,int(rand));
        count = (count%4)+1;  // 1<=count<=4
        ledCountMax = count;
        drawLine(tempaddr, count, c);  
        blockLine(tempaddr, count);

        while(!m.check())
        {
          game(50);
          if(ledCount==ledCountMax)
          {
            delay(500);
            break;
          }
        }
        ledCount = 0;
        ledReset();
        pixelReset();
        break;
      }

    case 2:    //drawDots
      {
        randomSeed(analogRead(0));
        long rand = random(100);
        int tempaddr = (int) random(0,int(rand));

        randomSeed(analogRead(0));
        rand = random(1, 872);
        int count = (int) random(0,int(rand));
        count = (count%4)+1;  // 1<=count<=4
        ledCountMax = count;

        drawDots(tempaddr, (int) rand, count, c);
        while(!m.check())
        {
          game(50);
          if(ledCount ==ledCountMax)
          {
            delay(500);
            break;
          }
        }
        ledCount = 0;  
        ledReset();
        pixelReset();
        break;
      }

    default:    //drawRectangle
      {
        randomSeed(analogRead(0));
        int w = (int) random(1,4); //1<=w<4
        int addr_x = (int) random(0,3); // 0<=addr_x<3
        int addr_y = (int) random(0,3); // 0<=addr_y<3;
        drawRectangle(w, addr_x, addr_y, c);
        blockSquare(w, w, addr_x, addr_y);
        ledCountMax = power(w,2);
        while(!m.check())
        {
          game(50);
          if(ledCount==ledCountMax) {
            delay(500);
            break;
          }
        }
        ledCount = 0;
        ledReset();
        pixelReset();
      }
    }  //End of switch~case    
  }
  Serial.println(" ");
  Serial.println("MINI ROUNDS");
  int mini_x = 2;
  int mini_y = 2;
  int mini_w = 1;

  for(int i=0; i<2; i++)
  {
    pixelReset();
    ledReset();

    drawRectangle(mini_w, mini_x, mini_y, c);
    blockSquare(mini_w, mini_w, mini_x, mini_y);
    ledCountMax = power(mini_w,2);

    Serial.print(ledCount);
    Serial.print(ledCountMax);

    while(!m.check()){
      game(50);
      if(ledCount==ledCountMax){
        delay(500);
        break;
      }
    }
    mini_x--;
    mini_y--;
    mini_w+=2;
    ledCount = 0;
  }

  pixelReset();
  ledReset();

  Serial.println("ROUND 1");
  for(int i =0; i<25; i++)    lightup(i, neutral);
  strip.show();
  while(ledCount!=25)     game(50);
  Serial.println("All LEDs are hit. Not taking any more inputs.");
  ledCount = 0;
  delay(1000);

  Serial.println("game...");
  delay(1000);
  pixelReset();
  ledReset();

  //put the ending sequence here
  endingSequence();
}

/*
  int check(int num) is the function that deals with muxes.
 It commands Arduino to check inputs from 0~num.
 First, Arduino saves the binary of the address.
 Then, by assigning each digit of the binary to each pin, 
 Arduino accesses the pin with the address.
 If a pin is hit, Arduino changes the states of the corresponding pixels.
 (Remember, there are two pins corresponding to a pin/LED. getPair(int n) is useful here)
 */
int game(int num)
{
  boolean soundcheck = true;
  int i = 0;
  while (i<num)
  {
    toBinary(i);
    digitalWrite(pinA, binary[0]);
    digitalWrite(pinB, binary[1]);
    digitalWrite(pinC, binary[2]);
    digitalWrite(pinD, binary[3]);
    digitalWrite(pinE, binary[4]);
    digitalWrite(pinF, binary[5]);    

    if(isHit())
//    int temp = ((int) testShot());  //testshot dummy
//    if(i == temp)    //testshot dummy
    {
      if((pixels[i].getState())==0)  //if hit by side 0
      {
        //        Serial.print(i);
        //        Serial.println("is hit,  ");
        //        Serial.println("Your button hasn't hit yet.");
        //        Serial.print(i);
        //        Serial.print(" and ");
        //        Serial.println(getPair(i));
        //First, set the two pixels that correspond to the LED as HIT
        pixels[i].setState(1);
        pixels[getPair(i)].setState(1);
        ledCount++;
        //        Serial.print("This is hit #");
        //        Serial.println(ledCount);

        //Then, record the hitter of the LED for the two corresponding pixels
        if(i>24)
        {
          pixels[i].setHitter(1);
          pixels[getPair(i)].setHitter(1);
          score_1 ++;
        }
        else {
          pixels[i].setHitter(0);
          pixels[getPair(i)].setHitter(0);
          score_0 ++;
        }        
        //        Serial.print("lighting up ");
        //        Serial.println(i);
        lightup_1(i);
        strip.show();
        Metro tHit = Metro(150);
        if(soundcheck){
          while(!tHit.check()) {
//            Serial.print("h  ");
//            digitalWrite(pinSound, HIGH);
            soundcheck = false;
          }
//          digitalWrite(pinSound, LOW);
        }
        break;
      }
      else{
        //                Serial.println("Your pin has hit already. Not doing anything.");
      }
    }
    i++;
    soundcheck = true;
  }        
}

/*
  boolean isHit() returns true if the corresponding target is 
 ever hit since the game started, false otherwise.
 */
boolean isHit()
{
  if(analogRead(pinZ) >100)    return true;
  else return false;
}

/*
  toBinary takes an integer n as a parameter, and returns the value in binary.
 For example, toBinary(16) returns 10000, toBinary(20) returns 10100.
 Since our project uses 6-digit binaries, datatype INT is not enough to store all values, 
 so an array that stores each digit of a binary to each slot is used.
 For example if toBinary(20) is called,
 binary[0] = 0, binary[1] = 1, binary[2] = 0, binary[3]=1, and so on.
 binary[0]= 0 because toBinary(20) returns 5-digit binary, while the array stores up to 6 digits.
 */

void toBinary(int n)
{
  int i = 0;
  int num = (sizeof(binary)/sizeof(int))-1;

  if(n !=0)  //this part is dangerous but works fine in arduino
    //as default int array is automatically initiallized with '0'.
  {
    while (n>1)
    {
      binary[num-i] = n%2;
      n = n/2;
      i++;
    }
    binary[num-i] = 1;
    if(i<=num)
    {
      for(i=i+1; i<=num; i++)    binary [num-i]=0;
    }
    else Serial.println("Your binary sucks, bitch.");    
  }
  else{
    for(int j =0; j<=num; j++)
    {
      binary[j] = 0;
    }
  }
}

/*
  int toDecimal() converts the existing binary stored in binary[] to decimal.
 */
int toDecimal()
{
  return (binary[0]*power(2,5))+(binary[1]*power(2,4))+(binary[2]*power(2,3))+(binary[3]*power(2,2))+(binary[4]*power(2,1))+(binary[5]*power(2,0));
}

/*
  void lightup(int n) lights up an LED with address n
 */

void lightup_1(int n)
{
  int addr = n%25;
  if(pixels[n].getSide())  //when address is larger than 24
  {
    lightup(addr, color_1);
  }
  else {
    lightup(addr,color_0);    
  }
}

void lightup(int n, LedColor c)
{
  strip.setPixelColor(((uint16_t)getx(n)),((uint16_t)gety(n)),c.getRed(),c.getGreen(),c.getBlue());
  strip.show();
}

/*address from 0-24, converts into coordinate value (x,y)
 Note: light strip "snakes" */
int getx(int n)
{
  int pixel = n+1;
  int absx = pixel%(strip.getWidth()); 
  int x = (absx+strip.getWidth()-1)%strip.getWidth(); 
  if((isEven(n) && n<=24)|| (!(isEven(n)) && n>24))  {   //goes reverse
    return ((strip.getWidth()-absx)+strip.getWidth())%strip.getWidth();
  }
  else return x;
}

int gety(int n){
  return n/(strip.getWidth());
}

/* isEven(int n) is a helper function for x. 
 Returns true if nth pixel is on the even row, false otherwise.
 */
boolean isEven (int n){
  return ((gety(n)%2)==1);
}

/*
  Modified version of pow(int base, int exponent) from pre-made Arduino's reference library.
 Because of the way the datatype float works, pre-made pow() goes only up to 10^4 or so.
 And as it returns float datatype, datacasting is always the pain in the ass. (Seriously, it is.)
 This power function goes up to 32768, or 10^5.
 So If you happen to use binary number that goes more than 7 digits, well... good luck, bro.
 I personally suggest using an array again to store decimals?
 */
int power (int base, int exponent)
{
  int value = 1;
  for(int i = 1; i<=exponent; i++) value*=base;
  return value;
}

/*
int getPair(int n) returns a corresponding address of a flipper.
 Addresses on the two sides are mirror-reflected. 
 */
int getPair(int n)
{
  int x, y;
  boolean side = 0; //0 if n<24
  if(n>24)  {
    side = 1;
    n= n-25;
  }
  y = n/5;
  x = n%5;
  if(side) 
  {
    return 5*y +x%5;
  }
  else   return 25+5*y + x;
}

long testShot()
{
  long rand = random(0,51);
  //  Serial.print("Random Shot:");
  //  Serial.println(rand);  
  return rand;
}

void ledReset()
{
  for(int i =0; i<25; i++)
  {
    strip.setPixelColor(getx(i), gety(i), 0, 0, 0);
    strip.show();
  }
}

LedColor getRandomColor()
{
  randomSeed(analogRead(0));
  int r = (int) random(0,170);
  int g = (int) random(0,180);
  int b = (int) random(0,170);

  return LedColor(r,g,b);
}

void drawRectangle(int w, int x, int y, LedColor c)
{
  int addr;
  int snake_addr;

  for(int j = 0; j<w; j++)
  {
    for(int i = 0; i<w; i++)
    {
      addr = (x+i) + ((y+j)*5);
      snake_addr = getx(addr) + gety(addr)*5;
      lightup(snake_addr%25,c);  
    }
    strip.show();
  }
}
void drawLine(int addr, int count, LedColor c)
{
  Serial.println("Draw Line");
  Serial.print("Snake Adress: ");
  Serial.println(addr%25);
  Serial.print("Starting from (");
  Serial.print(getx(addr));
  Serial.print(",");
  Serial.print(gety(addr)%5);
  Serial.print(") ");
  Serial.print(count);
  Serial.println(" pixels");
  Serial.print("lighting up ");
  for(int i = 0; i<count; i++)
  {
    Serial.print((i+addr)%25);
    Serial.print(",");
    lightup((i+addr)%25, c);
    strip.show();
  }
  Serial.println(" ");
}

void drawDots(int addr, int rand, int order, LedColor c)
{
  int toBlock[25];
  blockReset(toBlock);

  /*
  int duplicates[] array stores the address of dots Arduino's drawing.
   The purpose of this array is to prevent duplicates: Lighting up same pixles more than once.
   Pixel is not lighting up if false; pixel is already lighting up if true.
   Later, when Arduino tries to light up the pixel that is already on,
   it lights up its 7th neighbor.
   (7th is chosen as it seemed to be the most "random" arrangement as I could think)
   */
  boolean duplicates[25];
  for(int i = 0; i<25; i++)
  {
    duplicates[i] = false;
  }

  Serial.println("Draw Dots");
  Serial.println("Drawing...");
  for(int i = 0; i<order; i++)
  {
    if(duplicates[(addr%25)])
    {
      addr += 8;
      i--;
    }
    else{
      Serial.print(addr%25);
      Serial.print(" ");
      duplicates[(addr%25)] = true;
      lightup((addr%25),c);
      toBlock[addr%25] = 1;
      addr +=rand;
    }
  }
  Serial.println(" ");
  for(int i = 0; i<25; i++)
  {
    if(toBlock[i] == 0)
    {
      pixels[i].setState(1);
      pixels[getPair(i)].setState(1);
    }
    else {
      Serial.print(i);
      Serial.print(" ");
    }
  }
}

void pixelReset()
{
  for (int i=0; i<totnum;i++)
  {
    pixels[i] = Pixel(i);
    pixels[i].setAddress(i);
    if(i>24) {
      pixels[i].setSide(1);
    }
    else {
      pixels[i].setSide(0);
    }
    pixels[i].setState(0);
  }

}

void blockSquare(int w, int h, int x, int y)
{

  int toBlock[25];

  blockReset(toBlock);
  //pick pixels not to block
  int addr;
  int snake_addr; 
  for(int j=0; j<h; j++)
  {
    for(int i=0; i<w; i++)
    {
      addr = x+i+((y+j)*5);
      snake_addr = getx(addr)+(gety(addr)*5);
      toBlock[snake_addr] = 1;
    }
  }
  //block pixels from further taking inputs
  for(int i = 0; i<25; i++)
  {
    if(toBlock[i] == 0)
    {
      pixels[i].setState(1);
      pixels[getPair(i)].setState(1);

    }
    else 
    {
      pixels[i].setState(0);
      pixels[getPair(i)].setState(0);
    }
  }
}

/*
  abs-> snake to "proceed"
 snake->abs because block() uses abs coordinate
 */
void blockLine(int addr, int count)
{
  addr = addr%25;
  //  Serial.print("toBlock Address: ");
  //  Serial.println(addr);
  int toBlock[25];
  blockReset(toBlock);

  for(int i = 0; i<count; i++)
  {
    toBlock[(addr+i)%25] = 1;
  }

  for(int i =0; i<25; i++)
  {
    if(toBlock[i] ==0)
    {
      pixels[i].setState(1);
      pixels[getPair(i)].setState(1);
    }
    else {
      Serial.print(i);
      Serial.print(" ");
    }
  }
  Serial.println(" are not blocked");
}

void blockReset(int *toBlock)
{
  int length = strip.numPixels();

  //reset toBlock to default
  for(int i =0; i<length; i++)
  {
    toBlock[i] = 0;
  }
}
void idle()
{
  while(1){
    rainbow(5);
    rainbowCycle(5);
    colorWipe(Color(255, 0, 0), 50);
    colorWipe(Color(0, 255, 0), 50);
    colorWipe(Color(0, 0, 255), 50);     
    if(digitalRead(pinS) == HIGH) {
      Serial.println("Game Starts");
      Metro tsound = Metro (600);
      while(!tsound.check()) {
//        digitalWrite(pinSound, HIGH);
      }
//      digitalWrite(pinSound, LOW);
      break;
    }
  }
}
void startingSequence()
{
  drawColumn(0, 0, 5, LedColor(0, 255, 0), 1, 0);
  strip.show();
  drawColumn(1, 0, 5, LedColor(160, 120, 0), 1, 0);
  strip.show();
  drawColumn(2, 0, 5, LedColor(255, 0, 0), 1, 0);
  strip.show();
  drawColumn(3, 0, 5, LedColor(160, 120, 0), 1, 0);
  strip.show();
  drawColumn(4, 0, 5, LedColor(0, 255, 0), 1, 0);
  strip.show();
  delay(2000);

  drawColumn(0, 0, 5, LedColor(0, 0, 0), 1, 0);  
  drawColumn(4, 0, 5, LedColor(0, 0, 0), 1, 0);
  strip.show();
  delay(1000);
  drawColumn(1, 0, 5, LedColor(0, 0, 0), 1, 0);
  drawColumn(3, 0, 5, LedColor(0, 0, 0), 1, 0);
  strip.show();
  delay(1000);
  drawColumn(2, 0, 5, LedColor(0, 0, 0), 1, 0);
  strip.show();
  delay(1000);  
}

void rainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
      if(digitalRead(pinS) == HIGH) break;
    }  
    strip.show();   // write all the pixels out
    if(digitalRead(pinS) == HIGH) break;    
    delay(wait);
    if(digitalRead(pinS) == HIGH) break;
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
      if(digitalRead(pinS) == HIGH) break;
    }  
    strip.show();   // write all the pixels out
    delay(wait);
    if(digitalRead(pinS) == HIGH) break;
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    if(digitalRead(pinS) == HIGH) break;    
    delay(wait);
    if(digitalRead(pinS) == HIGH) break;
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170; 
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void endingSequence()
{
  Serial.println("end");
  Metro tsound = Metro (600);
  while(!tsound.check()) {    
//    digitalWrite(pinSound, HIGH);
  }
//  digitalWrite(pinSound, LOW);
     drawSpiral(true, LedColor(255, 255, 255));
     drawSpiral(false, LedColor(0, 255, 0));
  drawSpiral(true, LedColor(180, 40, 180));
  drawSpiral(false, LedColor(255, 154, 0));
  ledReset();
  centerBlink(LedColor(255, 0, 0), LedColor(0, 0, 255));

  //volume change here
  int ratio;  //Number of LEDs that will light up in BLUE
  LedColor winner = LedColor(123, 120, 90); //dummy color to track the error later
  
//  Serial.println("Ratio Fix Printcode");
//  Serial.print("Blue: ");
//  Serial.println(score_0);
//  Serial.print("Red: ");
//  Serial.println(score_1);
//  Serial.print("Ratio:");

  ratio = (score_0*25)/(score_1+score_0);
//  Serial.println(ratio);
  if(score_1>score_0)   {
    winner = color_1;  //verify this code
  }
  else if (score_0>score_1) {
    winner = color_0;  //verify this code
  }
  else {
    drawSpiral(false, draw);
    ratio = 12;
    winner = draw;
  }
  drawSpiral(true, winner);
  delay(2000);
  ledReset();
  int pix = 0;
//  Serial.print(ratio);
//  Serial.println(" pixels lighting up blue");
  while(pix<25)
  {
    if(pix<ratio)
    {
      lightup(pix, color_0);
      delay(50);
//      Serial.print(pix);
//      Serial.println("in blue");
    }
    else if(pix>=ratio)
    {
      lightup(pix, color_1);
      delay(50);
//      Serial.print(pix);
//      Serial.println("in red");
    }
    else if(pix == 12)
    {
      if(score_1 == score_0)
      {
        lightup(pix, draw);
        delay(50);
        pix++;
//        Serial.print(pix);
//        Serial.println("in draw");
      }
      else{
        lightup(pix, winner);
        delay(50);
//        Serial.print(pix);
//        Serial.println("in winner");
        pix++;
      }
    }
    pix++;
    strip.show();
  }
  delay(5000);
}

void centerBlink(LedColor c1, LedColor c2)
{
  int wait = 900;
  for(int i=0; i<4; i++)
  {
    for(int j = 0; j<4; j++)
    {
      strip.setPixelColor(2,2,c1.getRed(), c1.getGreen(), c1.getBlue());
      strip.show();
      delay(wait);
      strip.setPixelColor(2,2,c2.getRed(), c2.getGreen(), c2.getBlue());
      strip.show();
      delay(wait);
    }
    wait = wait - 300;
  }
}
void drawSpiral(boolean outward, LedColor c) //true for outward, false for inward
{
  boolean reserve = false; 
  int xx, yy, sign, xcount, ycount; 
  if(outward)
  {
    xx = 2; 
    yy = 2;
    sign = 1;
    xcount = 0;
    ycount = 0;
    while(1)
    {
      xcount++;
      drawRow(xx, yy, xcount, c, sign, 50);
      xx = xx+ sign*xcount;
      if(xcount == 5 || ycount ==5) reserve = true;
      if(reserve) break;
      if(xcount == 5 || ycount ==5) reserve = true;      
      ycount++;
      drawColumn(xx, yy, ycount, c, sign, 50);
      yy = yy+sign*ycount;
      sign = sign*(-1);
      if(xcount == 5 || ycount ==5) reserve = true;
      if(reserve) break;
    }
  }
  else{
    xx = 0;
    yy = 0;
    sign = 1;
    xcount = 5;
    ycount = 5; 
    while(1)
    {
      drawRow(xx, yy, xcount, c, sign, 50);
      xcount--;
      xx = xx + sign*xcount;
      yy = yy+sign; 
      if(xcount == 0) break;
      if(ycount == 5) ycount--;
      drawColumn(xx, yy, ycount, c, sign, 50);
      ycount --;

      yy = yy+sign*ycount;
      xx = xx-sign;
      sign = sign*(-1);
    }
  }
}

void drawRow(int x, int y, int count, LedColor c, int dir, int wait)
{
  for(int i = 0; i<count; i++)
  {
    if(dir>0){
      strip.setPixelColor(x+i, y, c.getRed(), c.getGreen(), c.getBlue());
      strip.show();
      delay(wait);
    }
    else {
      strip.setPixelColor(x-i, y, c.getRed(), c.getGreen(), c.getBlue());
      strip.show();
      delay(wait);
    }
  }
}

void drawColumn(int x, int y, int count, LedColor c, int dir, int wait)
{
  for(int i = 0; i<count; i++)
  {
    if(dir>0){
      strip.setPixelColor(x, y+i, c.getRed(), c.getGreen(), c.getBlue());
      strip.show();
      delay(wait);
    }
    else {
      strip.setPixelColor(x, y-i, c.getRed(), c.getGreen(), c.getBlue());
      strip.show();
      delay(wait);
    }
  }
}
