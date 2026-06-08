#include <Arduino.h>
#include "ldr.hpp"

#ifdef MODULE_LDR

// used for log approx.
#define VCC 3.3
#define R_FIXED 22000.0
#define LOGR_MIN log(1000.0)
#define LOGR_MAX log(400000.0)


void ldr::setup()
{
    #ifdef PIN_LDR_GND
    pinMode(PIN_LDR_GND, INPUT);  // set ground pin as input if required
    #endif
}


float ldr::measure()
{
    // read sensor value
    uint16_t value = analogRead(PIN_LDR);

    // calculate actual voltage
    float v = value * (VCC / 1023.0);

    if (v < 0.01) v = 0.01;
    if (v > VCC - 0.01) v = VCC - 0.01;

    float R_ldr = R_FIXED * (VCC / v - 1.0);

    // normalize resistance on log scale
    float logR = log(R_ldr);

    // approximate bounds
    float x = (logR - LOGR_MIN) / (LOGR_MAX - LOGR_MIN);

    if (x < 0.0) x = 0.0;
    if (x > 1.0) x = 1.0;

    // gamma correction (approx human perception)
    x = pow(x, 2.2);

    return x;
}
#endif
