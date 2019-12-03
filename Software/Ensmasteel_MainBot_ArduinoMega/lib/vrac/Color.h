#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED

//#include "Adafruit_TCS34725.h"
#include "1_CONSTANTS.h"
class Color
{
    public:
        Color(bool dummy);
        Color();
        //Adafruit_TCS34725 sensor;
        float red,green,blue;
        void raw();
        void actuate();
        PaletE getPaletCouleur();
    private:
        float tLastUpdate;

};

#endif // COLOR_INCLUDED
