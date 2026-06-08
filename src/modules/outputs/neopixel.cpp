#ifdef MODULE_NEO_PIXEL
#include "neopixel.hpp"

namespace outputs
{
    Adafruit_NeoPixel neo_pixel(1, 8, NEO_GRB + NEO_KHZ800);

    void set_pixel_color(uint32_t color)
    {
        neo_pixel.setPixelColor(0, color);
        neo_pixel.show();
    }

    void set_pixel_color(uint8_t r, uint8_t g, uint8_t b)
    {
        set_pixel_color(neo_pixel.Color(r, g, b));
    }
} // namespace outputs

#endif