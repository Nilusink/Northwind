/**
 * @file bmp388.hpp
 * @author Nilusink
 * @brief BMP 388 interface driver
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "Adafruit_BMP3XX.h"


namespace sensors
{
    class BMP388
    {
        private:
            // constants
            const uint32_t read_interval = 1000 * 10;  // every 10 seconds

            uint32_t last_update = -10000;
            Adafruit_BMP3XX bmp;

        public:
            BMP388();

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
            double get_pressure();
    };
} // namespace sensors
