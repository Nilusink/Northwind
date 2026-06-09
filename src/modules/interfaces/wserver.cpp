#include "wserver.hpp"

#ifdef MODULE_SHT31
#include "modules/sensors/sht31.hpp"
#endif

#ifdef MODULE_PBUTTON
#include "modules/outputs/pbutton.hpp"
#endif


namespace wserver
{
    #ifdef ESP8266
    ESP8266WebServer server(80);
    #endif
    #ifdef ESP32
    WebServer server(80);
    #endif

    JsonDocument weather_data;
    JsonDocument brightness_data;

    char error_msg[ERROR_MSG_MAXLEN];
} // namespace wserver


/* @section: endpoints */
char response_buffer[1024];
void wserver::handle_not_found() {
    server.send(404, "application/json", F("{\"error\": \"Not found\"}"));
}

void wserver::handle_error_msg()
{
    server.send(200, "text/plain", error_msg);
}

void wserver::handle_weather()
{
    serializeJson(weather_data, response_buffer);
    server.send(200, "application/json", response_buffer);
}

#ifdef MODULE_PBUTTON
void wserver::handle_blink_button()
{
    pbutton::blink();

    // send OK
    server.send(200, "text/plain", "OK");
}
#endif

#ifdef MODULE_LDR
void wserver::handle_brightness()
{
  serializeJson(brightness_data, response_buffer);
  server.send(200, "application/json", response_buffer);
}
#endif

/* @endsection */


void wserver::setup()
{
    Serial.println("server setup");
    clear_error_msg();

    // add endpoints
    #if defined(MODULE_SHT31) || defined(MODULE_BMP280) || defined(MODULE_CCS811) || defined(MODULE_TMP117) || defined(MODULE_BMP388) || defined(MODULE_SHT45) || defined(MAX31865)
    server.on("/weather", handle_weather);
    Serial.println("endpoint: /weather GET");
    #endif

    #ifdef MODULE_PBUTTON
    server.on("/pbutton", HTTP_POST, handle_blink_button);
    Serial.println("endpoint: /pbutton POST");
    #endif

    #ifdef MODULE_LDR
    server.on("/brightness", handle_brightness);
    Serial.println("endpoint: /brightness GET");
    #endif

    server.on("/traceback", handle_error_msg);
    server.onNotFound(handle_not_found);

    // start server
    server.begin();

    Serial.println("server done");
}


void wserver::set_error_msg(const char *msg)
{
    // copy message to buffer
    strncpy(error_msg, msg, ERROR_MSG_MAXLEN);

    // make sure null terminator exists
    error_msg[ERROR_MSG_MAXLEN-1] = 0;
}


void wserver::clear_error_msg()
{
    // reset error string
    memset(error_msg, 0, sizeof(error_msg));

    // set to no error
    set_error_msg("no errors.");
}
