/**
 * @file sht45.hpp
 * @author Nilusink
 * @brief sht45 interface
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifdef MODULE_SHT45
#include <SensirionI2cSht4x.h>


namespace sensors
{
    class SHT45
    {
        private:
            const float self_check_hum = 96;
            const uint32_t self_check_time = 5*60*1000;  // if RH > self-check-hum for 5mins
            const uint32_t read_interval = 1000 * 4;  // read interval in ms
            SensirionI2cSht4x  sht45;

            uint32_t over_since = 0;  // time over humidity threshold
            uint8_t curr_heater_power = 0;
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
            SHT45();

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
} // namespace sensors

#endif