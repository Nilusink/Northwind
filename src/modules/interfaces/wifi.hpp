/**
 * @file wifi.hpp
 * @author Nilusink
 * @brief manages the wifi connection (an addresses)
 * @version 0.1
 * @date 2025-11-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#endif

namespace interfaces
{
    class Wifi
    {
        private:
            HTTPClient client;
            WiFiClient wifiClient;

            IPAddress local_ip;
            IPAddress gateway;
            IPAddress subnet;
            IPAddress dns;

        public:
            Wifi();
            bool auto_connect();

            /**
             * @brief check network state
             * 
             */
            void update();

    };
}
