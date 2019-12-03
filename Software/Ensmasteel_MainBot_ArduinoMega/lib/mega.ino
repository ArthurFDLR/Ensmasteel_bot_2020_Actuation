#include "MegaServo.h"
#include "1_CONSTANTS.h"
#include "Mega.h"
#include "Arduino.h"

Mega mega;

void setup() 
{
  Serial.println("REBOOT");
  Serial.begin(250000);
  mega.init();

  while (mega.tirette.isPressed())
  {
    ;
  }
  Serial.println("fin du setup");
  
}

void loop() {
  mega.actuate();
  delay(1);
}
