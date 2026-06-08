/**
 * @file neopixel.hpp
 * @author Nilusink
 * @brief neopixel for debugging
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifdef MODULE_NEO_PIXEL
#include <Adafruit_NeoPixel.h>


namespace outputs
{
    extern Adafruit_NeoPixel neo_pixel;

    /**
     * @brief set the pixel color & show
     * 
     */
    void set_pixel_color(uint32_t color);
    void set_pixel_color(uint8_t r, uint8_t g, uint8_t b);
} // namespace outputs
#endif
