#ifdef MODULE_MAX31865
#include "max31865.hpp"


#define RNOMINAL 100

#ifdef MAX31865_RREF
#define RREF MAX31865_RREF
#else
#define RREF 430
#endif

using namespace sensors;


MAX31865::MAX31865(uint8_t cs_pin, uint8_t rdy_pin)
  : probe(cs_pin)
{}


void MAX31865::setup()
{
    probe.begin(MAX31865_2WIRE);
    probe.autoConvert(true);  // continuous mode
}


int MAX31865::read_sensor()
{

    // Check and print any faults
    fault = probe.readFault();
    if (fault)
    {
        Serial.print("Fault 0x"); Serial.println(fault, HEX);
        if (fault & MAX31865_FAULT_HIGHTHRESH) {
            Serial.println("RTD High Threshold"); 
        }
        if (fault & MAX31865_FAULT_LOWTHRESH) {
            Serial.println("RTD Low Threshold"); 
        }
        if (fault & MAX31865_FAULT_REFINLOW) {
            Serial.println("REFIN- > 0.85 x Bias"); 
        }
        if (fault & MAX31865_FAULT_REFINHIGH) {
            Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
        }
        if (fault & MAX31865_FAULT_RTDINLOW) {
            Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
        }
        if (fault & MAX31865_FAULT_OVUV) {
            Serial.println("Under/Over voltage"); 
        }
        probe.clearFault();

        return -1;  // return fail
    }
    last_temp = probe.temperature(RNOMINAL, RREF);

    return 0;  // return success
}


void MAX31865::update(uint32_t timestamp)
{
    // read if necessary
    if (timestamp - last_update > read_interval)
    {
        uint8_t i = 0;
        read_sensor();

        last_update = timestamp;
    }
}


float MAX31865::get_temperature()
{
    return last_temp;
}
#endif