#include "pbutton.hpp"
#ifdef MODULE_PBUTTON

namespace pbutton
{
    uint32_t blink_since = 0;
} // namespace pbutton



void pbutton::setup()
{
    Serial.println("power button setup");
    pinMode(D5, OUTPUT);
    digitalWrite(D5, 1);

    Serial.println("power button done");
}


void pbutton::blink()
{
    blink_since = millis();
    digitalWrite(D5, 0);
}


void pbutton::update(uint32_t timestamp)
{
    if (blink_since > 0 && (timestamp - blink_since) > BUTTON_BLINK_TIME)
    {
        blink_since = 0;
        digitalWrite(D5, 1);
    }
}
#endif
