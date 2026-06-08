#include "modules_manager.hpp"


using namespace modules;

ModulesManager::ModulesManager()
#ifdef MODULE_MAX31865
  : sensor_max(7, 3)
#endif
{}


void ModulesManager::setup()
{
    // give i2c time to setup
    delay(100);

    // connect OLED first to show status
    #ifdef MODULE_OLED
    oled.setup();
    delay(500);
    #endif

    // interfaces
    #ifdef MODULE_WIFI
    #ifdef MODULE_OLED
    oled.wifi_connecting();
    #endif

    if (!wifi.auto_connect())
    {
        #ifdef MODULE_OLED
        oled.wifi_fail();
        #endif
        for (;;);
    }

    #ifdef MODULE_WSERVER
    wserver::setup();
    #endif
    #endif

    // sensors
    #ifdef MODULE_OLED
    oled.setting_up();
    #endif

    #ifdef MODULE_TMP117
    delay(100);
    tmp.setup();
    delay(400);
    #endif

    #ifdef MODULE_SHT45
    delay(100);
    sht4x.setup();
    #endif

    #ifdef MODULE_SHT31
    delay(100);
    sht.setup();
    delay(400);
    #endif

    #ifdef MODULE_BMP388
    delay(100);
    bmp3.setup();
    delay(400);
    #endif

    #ifdef MODULE_LDR
    ldr::setup();
    #endif

    #ifdef MODULE_MAX31865
    sensor_max.setup();
    #endif

    // outputs
    #ifdef MODULE_PBUTTON
    pbutton::setup();
    #endif

    last_update = millis() - 1000;
}

void ModulesManager::update()
{
    uint32_t timestamp = millis();

    // read all possible values; for duplicate
    // sensor readings, the last one read is
    // saved; initial values to know which ones
    // have been read
    double 
        pressure = -999,
        temperature = -999,
        soil_temperature = -999,
        humidity = -999,
        brightness = 0.5;
    
    uint16_t
        co2 = UINT16_MAX,
        tvoc = UINT16_MAX;

    // only update sensors every 100ms
    if (timestamp - last_update > SENSOR_UPDATE_RATE)
    {
        // update all modules
        #ifdef MODULE_TMP117
        tmp.update(timestamp);
        #endif
        #ifdef MODULE_SHT45
        sht4x.update(timestamp);
        #endif
        #ifdef MODULE_SHT31
        sht.update(timestamp);
        #endif
        #ifdef MODULE_BMP388
        bmp3.update(timestamp);
        #endif
        #ifdef MODULE_MAX31865
        sensor_max.update(timestamp);
        #endif

        #ifdef MODULE_NEO_PIXEL
        outputs::neo_pixel.setPixelColor(0, outputs::neo_pixel.Color(0, 0, 1));
        outputs::neo_pixel.show();
        #endif
        last_update = timestamp;

        // brightness
        #ifdef MODULE_LDR
        brightness = ldr::measure();
        #endif

        // temperature
        #ifdef MODULE_BMP388
        temperature = bmp3.get_temperature();
        delay(10);
        #endif
        #ifdef MODULE_SHT31
        if (sht.get_status())
        {
            temperature = sht.get_temperature();
        }
        delay(10);
        #endif
        #ifdef MODULE_SHT45
        if (sht4x.get_status())
        {
            temperature = sht4x.get_temperature();
        }
        delay(10);
        #endif
        #ifdef MODULE_TMP117
        temperature = tmp.get_temperature();
        delay(10);
        #endif

        // humidity
        #ifdef MODULE_SHT31
        if (sht.get_status())
        {
            humidity = sht.get_humidity();
        }
        delay(10);
        #endif
        #ifdef MODULE_SHT45
        if (sht4x.get_status())
        {
            humidity = sht4x.get_humidity();
        }
        delay(10);
        #endif

        // air pressure
        #ifdef MODULE_BMP388
        pressure = bmp3.get_pressure();
        delay(10);
        #endif

        // soil temperature
        #ifdef MODULE_MAX31865
        soil_temperature = sensor_max.get_temperature();
        #endif

        // CO2

        // TVOC

        // OLED
        #ifdef MODULE_OLED
        oled.update(timestamp, temperature, humidity, pressure, co2, tvoc);
        #endif

        // power button
        #ifdef MODULE_PBUTTON
        pbutton::update(timestamp);
        #endif

        #ifdef MODULE_SERIAL_DEBUG
        Serial.printf(
            "Data: temp=%06.2f°C, hum=%05.1f%%, pressure=%05.1fhPa, soil=%06.2f°C\n",
            temperature,
            humidity,
            pressure,
            soil_temperature
        );
        #endif

        #ifdef MODULE_NEO_PIXEL
        outputs::neo_pixel.setPixelColor(0, outputs::neo_pixel.Color(0, 0, 0));
        outputs::neo_pixel.show();
        #endif

        #ifdef MODULE_WSERVER
        // clear json documents
        wserver::brightness_data.clear();
        wserver::weather_data.clear();

        // assign measured values
        wserver::brightness_data["brightness"] = brightness;
        if (temperature > -900)
        {
            wserver::weather_data["temperature"] = temperature;
        }
        if (humidity > -900)
        {
            wserver::weather_data["humidity"] = humidity;
        }
        if (pressure > -900)
        {
            wserver::weather_data["pressure"] = pressure;
        }
        if (soil_temperature > -900)
        {
            wserver::weather_data["soil_temperature"] = soil_temperature;
        }
        #endif
    }

    // check for wifi status
    #ifdef MODULE_WIFI
    wifi.update();
    #endif

    // wserver
    #ifdef MODULE_WSERVER
    // handle clients
    wserver::server.handleClient();
    yield();
    #endif
}
