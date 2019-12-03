#include "Color.h"

Color::Color()
{
    //ctor
}

Color::Color(bool dummy)
{
    sensor= Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
    sensor.begin();
    tLastUpdate=millis()/1000.0;
}

void Color::actuate()
{
    if (millis()/1000.0-tLastUpdate>0.1)
    {
        sensor.getRGB(&red, &green, &blue);
    }
}

void Color::raw()
{
    Serial.print("Rouge ");Serial.print(red);Serial.print("\tVert ");Serial.print(green);Serial.print("\tBleu ");Serial.println(blue);
}

#define SEUIL_ROUGE 1.2
#define SEUIL_VERT  1.2
#define SEUIL_BLEU  1.2
PaletE Color::getPaletCouleur()
{
    if (red/green>SEUIL_ROUGE && red/blue>SEUIL_ROUGE)
        return PaletE::Rouge;
    else if(green/red>SEUIL_VERT && green/blue>SEUIL_VERT)
        return PaletE::Vert;
    else if (blue/red>SEUIL_BLEU && blue/green>SEUIL_BLEU)
        return PaletE::Bleu;
    else
        return PaletE::VIDE;
}
