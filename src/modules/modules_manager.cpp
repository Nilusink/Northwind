#include "modules_manager.hpp"
#include "modules/interfaces/wifi_config.h"


using namespace modules;

ModulesManager::ModulesManager()
#if defined(MODULE_MAX31865) || defined(MODULE_WIFI)
 : 
#endif
#ifdef MODULE_WIFI
    connection(IPAddress(MANAGER_IP), MANAGER_PORT, IPAddress(GATEWAY), IPAddress(SUBNET), IPAddress(DNS))
#ifdef MODULE_MAX31865
    ,
#endif
#endif
#ifdef MODULE_MAX31865
  sensor_max(7, 3)
#endif
{}


void ModulesManager::setup()
{
    // setup debug neopixel
    #ifdef MODULE_NEO_PIXEL
    outputs::neo_pixel.begin();
    outputs::neo_pixel.setPixelColor(0, outputs::neo_pixel.Color(32, 32, 63));
    outputs::neo_pixel.show();
    #endif

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

    if (!connection.initialize(SSID, PASSWORD))
    {
        #ifdef MODULE_OLED
        oled.wifi_fail();
        #endif

        for (;;);
    }

    #ifdef MODULE_WSERVER
    wserver::setup();

    // create connectivity task
    xTaskCreate(
        connectivity_task_wrapper,
        "connectivity task",
        8192,
        this,
        1,
        NULL
    );
    #endif

    // create AuroraCOnnect task
    connection.start_task();
    #endif

    #ifdef MODULE_NEO_PIXEL
    outputs::neo_pixel.begin();
    outputs::neo_pixel.setPixelColor(0, outputs::neo_pixel.Color(63, 63, 0));
    outputs::neo_pixel.show();
    #endif

    // sensors
    #ifdef MODULE_OLED
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("OLED failed");
    #endif
    oled.setting_up();
    #endif

    #ifdef MODULE_TMP117
    delay(100);
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("TMP117 failed");
    #endif
    tmp.setup();
    delay(400);
    #endif

    #ifdef MODULE_SHT45
    delay(100);
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("SHT4x failed");
    #endif
    sht4x.setup();
    #endif

    #ifdef MODULE_SHT31
    delay(100);
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("SHT3x failed");
    #endif
    sht.setup();
    delay(400);
    #endif

    #ifdef MODULE_BMP388
    delay(100);
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("BMP3xx failed");
    #endif
    bmp3.setup();
    delay(400);
    #endif

    #ifdef MODULE_LDR
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("LDR failed");
    #endif
    ldr::setup();
    #endif

    #ifdef MODULE_MAX31865
    #ifdef MODULE_WSERVER
    wserver::set_error_msg("MAX31865 failed");
    #endif
    sensor_max.setup();
    #endif

    // outputs
    #ifdef MODULE_PBUTTON
    pbutton::setup();
    #endif

    last_update = millis() - 1000;

    #ifdef MODULE_NEO_PIXEL
    outputs::neo_pixel.setPixelColor(0, outputs::neo_pixel.Color(0, 0, 0));
    outputs::neo_pixel.show();
    #endif

    #ifdef MODULE_WSERVER
    wserver::clear_error_msg();
    #endif

}

void ModulesManager::update_sensors()
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

    vTaskDelay(pdMS_TO_TICKS(SENSOR_UPDATE_RATE / 10));
}


void ModulesManager::update_connectivity()
{
    // wserver
    #ifdef MODULE_WSERVER
    // handle clients
    if (connection.get_connected())
    {
        wserver::server.handleClient();
    }
    yield();
    #endif
}


void ModulesManager::create_tasks()
{
    // create sensor task
    xTaskCreate(
        sensors_task_wrapper,
        "sensor task",
        4096,
        this,
        1,
        NULL
    );
}


void ModulesManager::sensors_task_wrapper(void *pv) {
    ModulesManager *obj = static_cast<ModulesManager *>(pv);

    // run continuously
    for (;;)
    {
        obj->update_sensors();
        // Serial.printf(
        //     "Remaining s stack: %u\n",
        //     uxTaskGetStackHighWaterMark(NULL)
        // );
    }
}

void ModulesManager::connectivity_task_wrapper(void *pv) {
    ModulesManager *obj = static_cast<ModulesManager *>(pv);

    // run continuously
    for (;;)
    {
        obj->update_connectivity();
        // Serial.printf(
        //     "Remaining c stack: %u\n",
        //     uxTaskGetStackHighWaterMark(NULL)
        // );
    }
}
