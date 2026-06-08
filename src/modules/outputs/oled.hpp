/**
 * @file oled.hpp
 * @author Nilusink
 * @brief simple oled display for the HAS v2
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <Arduino.h>
#include <U8g2lib.h>


#define GRAPH_WIDTH 128
#define GRAPH_HEIGHT 44
#define GRAPH_X 0
#define GRAPH_Y 20


namespace outputs
{
    class OLED
    {
        private:
            U8G2_SH1106_128X64_NONAME_F_HW_I2C oled;
            double last_data[GRAPH_WIDTH];
            uint8_t curr_pop;

            void draw_graph(uint32_t timestamp, double temperature);
            const uint32_t graph_interval = 2000;  // read interval in ms
            uint32_t last_update = -10000;

        public:
            OLED();
            void setup();
            void update(
                uint32_t timestamp,
                double temperature,
                double humidity,
                uint16_t pressure,
                uint16_t co2,
                uint16_t tvoc
            );

            void wifi_connecting();
            void wifi_fail();
            void setting_up();
    };
}