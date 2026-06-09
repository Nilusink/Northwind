/**
 * @file modules_manager.hpp
 * @author Nilusink
 * @brief manages all modules in one unified data output
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
// sensors
#ifdef MODULE_TMP117
#include "modules/sensors/tmp117.hpp"
#endif

#ifdef MODULE_SHT31
#include "modules/sensors/sht31.hpp"
#endif

#ifdef MODULE_SHT45
#include "modules/sensors/sht45.hpp"
#endif

#ifdef MODULE_BMP388
#include "modules/sensors/bmp388.hpp"
#endif

#ifdef MODULE_LDR
#include "modules/sensors/ldr.hpp"
#endif

#ifdef MODULE_MAX31865
#include "modules/sensors/max31865.hpp"
#endif

// outputs
#ifdef MODULE_OLED
#include "modules/outputs/oled.hpp"
#endif

#ifdef MODULE_PBUTTON
#include "modules/outputs/pbutton.hpp"
#endif

#ifdef MODULE_NEO_PIXEL
#include "modules/outputs/neopixel.hpp"
#endif

// interfaces
#ifdef MODULE_WIFI
#include "modules/interfaces/wifi.hpp"
#endif

#ifdef MODULE_WSERVER
#include "modules/interfaces/wserver.hpp"
#endif


namespace modules
{
    class ModulesManager
    {
        private:
            uint32_t last_update;

            #ifdef MODULE_TMP117
            sensors::TMP117 tmp;
            #endif
            #ifdef MODULE_SHT45
            sensors::SHT45 sht4x;
            #endif
            #ifdef MODULE_SHT31
            sensors::SHT31 sht;
            #endif
            #ifdef MODULE_BMP388
            sensors::BMP388 bmp3;
            #endif
            #ifdef MODULE_MAX31865
            sensors::MAX31865 sensor_max;
            #endif
            #ifdef MODULE_OLED
            outputs::OLED oled;
            #endif
            #ifdef MODULE_WIFI
            interfaces::Wifi wifi;
            #endif


            static void sensors_task_wrapper(void *pv);
            static void connectivity_task_wrapper(void *pv);

            /** update all hardware modules */
            void update_sensors();
            /** update wifi + server */
            void update_connectivity();

        public:
            ModulesManager();

            /**
             * @brief setup all modules
             *
             */
            void setup();

            /**
             * @brief create all necessary tasks
             * 
             */
            void create_tasks();
    };
}
