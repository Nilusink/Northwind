#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "wifi.hpp"
#include "wifi_config.h"

#ifdef MODULE_NEO_PIXEL
#include "modules/outputs/neopixel.hpp"
#endif


using namespace interfaces;

Wifi::Wifi()
  : gateway(GATEWAY), subnet(SUBNET), dns(DNS)
{}


bool Wifi::auto_connect()
{
    WiFi.begin(SSID, PASSWORD);

    // wait for wifi to connect
    Serial.println("Connecting ...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.print("\nConnected with IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println(", configurint static ip...");

    // request ip for device id
    JsonDocument doc;
    char buff[64];
    snprintf(buff, 63, "%s/device/%d/address", MANAGER_ADDRESS, DEVICE_ID);
    if (client.begin(wifiClient, buff))
    {
        int http_code = client.GET();

        // check for valid response
        if (http_code > 0)
        {
            if (http_code == HTTP_CODE_OK)
            {
                String payload = client.getString();
                
                // convert to json
                DeserializationError error = deserializeJson(doc, payload);

                 // Check for parsing errors
                if (error) {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                    return false;
                }
            }
            else
            {
                Serial.printf("HTTP error: %d\n", http_code);
                return false;
            }
        }
        else
        {
            // Falls HTTP-Error
            Serial.printf("HTTP-Error: %s\n", client.errorToString(http_code).c_str());
            return false;
        }
    }
    else
    {
        Serial.println("couldn't connect to IOTManager!");
        return false;
    }

    // set static ip
    String static_ip = doc["ip"];
    local_ip.fromString(static_ip);

    Serial.printf("got assigned ip: %s\n", static_ip.c_str());

    if (!WiFi.config(local_ip, gateway, subnet, dns))
    {
        Serial.println("Failed to configure static IP");
    }
    Serial.println("reconfigured static ip");

    // wait for wifi to connect
    Serial.println("Connecting ...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    // enable OTA
    ArduinoOTA.begin();

    // update terminal
    Serial.print("\nConnected with IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}


void Wifi::update()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        #ifdef MODULE_NEO_PIXEL
        outputs::set_pixel_color(64, 64, 64);  // white on wifi error
        bool curr_on = true;
        #endif
        // reset wifi
        WiFi.disconnect();

        // try to re-connect
        while (!auto_connect())
        {
            delay(500);

            // blink neopixel white
            #ifdef MODULE_NEO_PIXEL
            if (curr_on)
            {
                outputs::set_pixel_color(0, 0, 0);
            }
            else
            {
                outputs::set_pixel_color(64, 64, 64);
            }
            curr_on = !curr_on;
            #endif
        }

        // turn of LED
        #ifdef MODULE_NEO_PIXEL
        outputs::set_pixel_color(0, 0, 0);
        #endif
    }
    else
    {
        // handle OTA
        ArduinoOTA.handle();
    }
}
