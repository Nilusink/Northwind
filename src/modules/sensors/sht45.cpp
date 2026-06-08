#ifdef MODULE_SHT45
#include <Wire.h>
#include "sht45.hpp"


// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

using namespace sensors;


void SHT45::check_health(uint32_t timestamp)
{
    if (last_hum >= self_check_hum)
    {
        // self-check if humidity has been above threshold
        // for more than self-check-time ms
        if (timestamp - over_since > self_check_time)
        {
            Serial.println("SHT45 self-check...");

            // initial read
            float
                t = 0.0,
                h = 0.0,
                nt = 0.0,
                nh = 0.0;

            sht45.measureMediumPrecision(t, h);

            // read new values with heater
            if (curr_heater_power == 0)
            {
                sht45.activateLowestHeaterPowerLong(nt, nh);
            }
            else if (curr_heater_power == 1)
            {
                sht45.activateMediumHeaterPowerLong(nt, nh);
            }
            else
            {
                sht45.activateHighestHeaterPowerLong(nt, nh);
            }

            // check for consistency
            if ( (nt-t < .2) || (h-nh < 1))
            {
                Serial.println("!!!!!!!!!!!!!  SHT45 self-check FAILED  !!!!!!!!!!!!!!!!!!!!");
                is_okay = false;

                if (curr_heater_power < 2)
                    curr_heater_power++;
            }
            else
            {
                Serial.println("!!!!!!!!!!!!!  SHT45 self-check PASS  !!!!!!!!!!!!!!!!!!!!");
                is_okay = true;
            }
        }
    }
    else
    {
        // reset time over humidity treshold if humidity is not above threshold
        over_since = timestamp;
        is_okay = true;
    }
}


SHT45::SHT45()
  : sht45()
{}


static char errorMessage[64];
static int16_t error;

void SHT45::setup()
{
    Serial.println("SHT45 setup");
    sht45.begin(Wire, SHT40_I2C_ADDR_44);
    sht45.softReset();
 
    // try to get serial number to check if sensor is available
    delay(10);
    uint32_t serialNumber = 0;
    error = sht45.serialNumber(serialNumber);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute serialNumber(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        for (;;) {delay(10);};
    }

    Serial.print("SHT45 serialNumber: ");
    Serial.print(serialNumber);
    Serial.println();
}


void SHT45::update(uint32_t timestamp)
{
    // check if the sensor is OK
    check_health(timestamp);

    // read if necessary
    if (timestamp - last_update > read_interval)
    {
        // measure sensor
        error = sht45.measureHighPrecision(last_temp, last_hum);

        uint8_t i = 0;
        while (error != NO_ERROR && i < 10)
        {
            // if error occured, try to re-measure up to 10 times
            error = sht45.measureHighPrecision(last_temp, last_hum);

            delay(50);
            i++;
        }

        // print error if one occured
        if (error != NO_ERROR) {
            Serial.print("Error trying to execute measureHighPrecision(): ");
            errorToString(error, errorMessage, sizeof errorMessage);
            Serial.println(errorMessage);
            return;
        }

        last_update = timestamp;
    }
}


float SHT45::get_temperature()
{
    return last_temp;
}
float SHT45::get_humidity()
{
    return last_hum;
}
bool SHT45::get_status()
{
    return is_okay;
}
#endif
