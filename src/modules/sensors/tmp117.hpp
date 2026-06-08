/**
 * @file tmp117.hpp
 * @author Nilusink
 * @brief tmp117 interface
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifdef MODULE_TMP117
#include <Adafruit_Sensor.h>
#include <Adafruit_TMP117.h>
#include <Wire.h>


namespace sensors
{
    class TMP117
    {
        private:
            // constants
            const uint32_t read_interval = 1000 * 8;  // every 8 seconds

            uint32_t last_update = -10000;
            double last_temp = -999;
            Adafruit_TMP117 tmp;

        public:
            TMP117();

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

            double get_temperature();
    };
} // namespace sensors
#endif
