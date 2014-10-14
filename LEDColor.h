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
