#ifdef MODULE_TMP117
#include "tmp117.hpp"

using namespace sensors;

TMP117::TMP117()
  : tmp()
{};


void TMP117::setup()
{
    int i = 0;
    // connect to sensor
    while (!tmp.begin())
    {
        Serial.println("Couldn't find TMP117");
        i++;
        delay(200);
    
        if (i >= 10)
        {
            while (1) delay(1);
        }
    }

    // configure sensor
    tmp.setAveragedSampleCount(TMP117_AVERAGE_32X);
    tmp.setReadDelay(TMP117_DELAY_8000_MS);
    tmp.setMeasurementMode(TMP117_MODE_CONTINUOUS);
}


void TMP117::update(uint32_t timestamp)
{
    if (tmp.dataReady())
    {
        sensors_event_t temp_event;
        tmp.getEvent(&temp_event);
        last_temp = temp_event.temperature;
    }
}


double TMP117::get_temperature()
{
    return last_temp;
}
#endif
