#include <Arduino.h>
#include <Wire.h>
#include "modules/modules_manager.hpp"

#ifdef MODULE_NEO_PIXEL
#include "modules/outputs/neopixel.hpp"
#endif

modules::ModulesManager mgr;


void setup()
{
    // setup i2c and Serial
    Serial.begin(9600);
    while (!Serial) {delay(10);}

    #ifdef ESP32
    delay(2000);
    #endif

    Serial.println(DEVICE_ID);

    #ifdef PIN_SDA
    Wire.begin(PIN_SDA, PIN_SCL);
    #else
    Wire.begin();
    #endif

    // setup devices
    mgr.setup();

    // create tasks
    mgr.create_tasks();
}

void loop() {delay(10);};
