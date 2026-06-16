#ifdef MODULE_MAX31865
#include "max31865.hpp"
#ifdef MODULE_WSERVER
#include "modules/interfaces/wserver.hpp"
#endif


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

        #ifdef MODULE_WSERVER
        if (fault == 0x00 || fault == 0xFF)
        {
            wserver::set_error_msg("MAX31865 SPI read failure");
        }
        else
        {
            bool set = false;
            if (fault & MAX31865_FAULT_HIGHTHRESH) {
                wserver::set_error_msg("RTD High Threshold");
                set = true;
            }
            if (fault & MAX31865_FAULT_LOWTHRESH) {
                wserver::set_error_msg("RTD Low Threshold"); 
                set = true;
            }
            if (fault & MAX31865_FAULT_REFINLOW) {
                wserver::set_error_msg("REFIN- > 0.85 x Bias"); 
                set = true;
            }
            if (fault & MAX31865_FAULT_REFINHIGH) {
                wserver::set_error_msg("REFIN- < 0.85 x Bias - FORCE- open"); 
                set = true;
            }
            if (fault & MAX31865_FAULT_RTDINLOW) {
                wserver::set_error_msg("RTDIN- < 0.85 x Bias - FORCE- open"); 
                set = true;
            }
            if (fault & MAX31865_FAULT_OVUV) {
                wserver::set_error_msg("Under/Over voltage");
                set = true;
            }
            if (!set)
            {
                snprintf(wserver::error_msg, ERROR_MSG_MAXLEN, "unkown fault code: %02X", fault);
            }
        }
        #endif

        probe.clearFault();

        return -1;  // return fail
    }
    float now = probe.temperature(RNOMINAL, RREF);

    // check for invalid readings
    if (now < -100)
        return -1;

        last_temp = now;

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