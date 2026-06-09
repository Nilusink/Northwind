/**
 * @file wserver.hpp
 * @author Nilusink
 * @brief webserver interface
 * @version 0.1
 * @date 2026-05-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#ifdef ESP8266
#include <ESP8266WebServer.h>
#endif
#ifdef ESP32
#include <WebServer.h>
#endif

#include <ArduinoJson.h>


#define ERROR_MSG_MAXLEN 2048

namespace wserver
{
    #ifdef ESP8266
    extern ESP8266WebServer server;
    #endif
    #ifdef ESP32
    extern WebServer server;
    #endif

    extern JsonDocument weather_data;
    extern JsonDocument brightness_data;
    extern char error_msg[ERROR_MSG_MAXLEN];

    // endpoints
    void handle_not_found();
    void handle_weather();
    void handle_error_msg();
    
    #ifdef MODULE_PBUTTON
    void handle_blink_button();
    #endif

    #ifdef MODULE_LDR
    void handle_brightness();
    #endif

    /**
     * @brief setup server
     * 
     */
    void setup();

    /**
     * @brief set error message
     * 
     */
    void set_error_msg(const char *msg);
    void clear_error_msg();
} // namespace wserver
