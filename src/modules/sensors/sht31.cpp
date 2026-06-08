#include <Wire.h>
#include "sht31.hpp"


using namespace sensors;


void SHT31::check_health(uint32_t timestamp)
{
    if (last_hum >= self_check_hum)
    {
        // self-check if humidity has been above threshold
        // for more than self-check-time ms
        if (timestamp - over_since > self_check_time)
        {
            Serial.println("SHT31 self-check...");

            // initial read
            float t, h, nt, nh;
            sht31.readBoth(&t, &h);

            // set heater on for 2 seconds
            sht31.heater(true);
            delay(2000);

            // read new values
            sht31.readBoth(&nt, &nh);
            sht31.heater(false);

            // check for consistency
            if ( (nt-t < .2) || (h-nh < 1))
            {
                Serial.println("!!!!!!!!!!!!!  SHT31 self-check FAILED  !!!!!!!!!!!!!!!!!!!!");
                is_okay = false;
            }
            else
            {
                Serial.println("!!!!!!!!!!!!!  SHT31 self-check PASS  !!!!!!!!!!!!!!!!!!!!");
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


SHT31::SHT31()
  : sht31()
{}


void SHT31::setup()
{
    Serial.println("SHT31 setup");
    delay(10);
    int i = 0;
    while (!sht31.begin()) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
        i++;
        delay(200);
    
        if (i >= 10)
        {
            while (1) delay(1);
        }
    }
    sht31.heater(false);
    Serial.println("SHT31 done");
}


void SHT31::update(uint32_t timestamp)
{
    // check if the sensor is OK
    check_health(timestamp);

    // read if necessary
    if (timestamp - last_update > read_interval)
    {
        // sht31.readStatus();
        uint8_t i = 0;
        while (!sht31.readBoth(&last_temp, &last_hum) && i < 10)
        {
            delay(50);
            i++;
        }

        last_update = timestamp;
    }
}


float SHT31::get_temperature()
{
    return last_temp;
}
float SHT31::get_humidity()
{
    return last_hum;
}
bool SHT31::get_status()
{
    return is_okay;
}