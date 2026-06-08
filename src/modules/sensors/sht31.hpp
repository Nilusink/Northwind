/**
 * @file sht31.hpp
 * @author Nilusink
 * @brief sht31 sensor interface
 * @version 0.1
 * @date 2025-10-26
 * 
 * @copyright Copyright Nilusink (c) 2025
 * 
 */
#pragma once
#include <Wire.h>
#include <Adafruit_SHT31.h>


namespace sensors
{
    class SHT31
    {
        private:
            const float self_check_hum = 96;
            const uint32_t self_check_time = 5*60*1000;  // if RH > self-check-hum for 5mins
            const uint32_t read_interval = 2000;  // read interval in ms
            Adafruit_SHT31 sht31;

            uint32_t over_since = 0;  // time over humidity threshold
            uint32_t last_update = -10000;
            float last_temp = -999;
            float last_hum = -1;
            bool is_okay = true;

            /**
             * @brief self-tests using the internal heater if necessary
             * 
             */
            void check_health(uint32_t timestamp);

        public:
            SHT31();

            /**
             * @brief initial setup for the sensor
             * 
             */
            void setup();

            /**
             * @brief update both sensor values and heater control
             * 
             */
            void update(uint32_t timestamp);

            float get_temperature();
            float get_humidity();

            /**
             * @brief returns if the sensor is currently working
             * 
             * @return true okay
             * @return false fail
             */
            bool get_status();
    };
}