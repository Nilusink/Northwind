/**
 * @file pbutton.hpp
 * @author Nilusink
 * @brief power button module (blink pin to 0)
 * @version 0.1
 * @date 2026-05-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#include <Arduino.h>

#ifdef MODULE_PBUTTON
#define BUTTON_BLINK_TIME 100


namespace pbutton
{
    extern uint32_t blink_since;

    void setup();
    void blink();
    void update(uint32_t timestamp);
} // namespace pbutton
#endif
