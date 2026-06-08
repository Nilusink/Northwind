/**
 * @file max31865.hpp
 * @author Nilusink
 * @brief max31865 interface
 * @version 0.1
 * @date 2026-06-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifdef MODULE_MAX31865
#include <Adafruit_MAX31865.h>

namespace sensors
{
        class MAX31865
    {
        private:
            // constants
            const uint32_t read_interval = 1000 * 8;  // every 8 seconds

            uint32_t last_update = -10000;
            float last_temp = -999;
            Adafruit_MAX31865 probe;
            uint8_t fault;

            /**
             * @brief prompt sensor read
             * 
             * @param temperature [out] temperature
             * @return int 1 if OK, 0 if FAIL
             */
            int read_sensor();

        public:
            MAX31865(uint8_t cs_pin, uint8_t rdy_pin);

            /**
             * @brief initialize the sensor
             * 
             */
            void setup();

            /**
             * @brief update sensor values
             * 
             */
            void update(uint32_t timestamp);

            float get_temperature();
    };
} // namespace sensors

#endif
