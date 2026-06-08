#include "oled.hpp"


// utility functions
double d_map(double x, double in_min, double in_max, double out_min, double out_max)
{
    // magic
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


double d_max(double arr[], uint8_t length=GRAPH_WIDTH)
{
    double curr_max = arr[0];
    for (int i = 1; i < length; i++)
    {
        if (arr[i] > curr_max)
        {
            curr_max = arr[i];
        }
    }

    return curr_max;
}

double d_min(double arr[], uint8_t length=GRAPH_WIDTH)
{
    double curr_min = arr[0];
    for (int i = 1; i < length; i++)
    {
        if (arr[i] < curr_min)
        {
            curr_min = arr[i];
        }
    }

    return curr_min;
}


using namespace outputs;

OLED::OLED()
  : oled(U8G2_R0)
{
    curr_pop = 0;
};

void OLED::setup()
{
    oled.begin();
    Serial.println("OLED done");
}

void OLED::update(
    uint32_t timestamp,
    double temperature,
    double humidity,
    uint16_t pressure,
    uint16_t co2,
    uint16_t tvoc
)
{
    oled.firstPage();

    // temperature, humidity + air quality
    if (humidity > -999 && tvoc != UINT16_MAX)
    {
        oled.setFont(u8g2_font_ncenB14_tr);
        char buff[64];

        sprintf(buff, "%.1f C", temperature);
        oled.drawStr(40, 17, buff);

        sprintf(buff, "%.1f %s", humidity, "%");
        oled.drawStr(40, 40, buff);

        oled.setFont(u8g2_font_ncenB08_tr);

        if (tvoc <= 65)
        {
            oled.drawStr(13, 60, "Air Quality:");
            oled.drawStr(86, 60, "Good");
        }
        else if (tvoc <= 220)
        {
            oled.drawStr(13, 60, "Air Quality:");
            oled.drawStr(86, 60, "Okay");
        }
        else if (tvoc <= 660)
        {
            oled.drawStr(8, 60, "Air Quality:");
            oled.drawStr(82, 60, "Stuffy!");
        }
        else if (tvoc <= 2200)
        {
            oled.drawStr(12, 60, "Air Quality:");
            oled.drawStr(84, 60, "Poor!");
        }
        else
        {
            oled.drawStr(1, 60, "Air Quality:");
            oled.drawStr(68, 60, "Very Poor!");
        }
    }
    // temperature + humidity
    else if (humidity > -999)
    {
        oled.setFont(u8g2_font_ncenB08_tr);
        char buff[64];

        sprintf(buff, "%.1f C", temperature);
        oled.drawStr(16, (uint8_t)(GRAPH_Y / 2), buff);

        sprintf(buff, "%.1f %s", humidity, "%");
        oled.drawStr(82, (uint8_t)(GRAPH_Y / 2), buff);

        oled.drawTriangle(52, 0, 76, 0, 64, 12);

        draw_graph(timestamp, temperature);
    }
    // temperature only
    else if (temperature > -999)
    {
        oled.setFont(u8g2_font_ncenB14_tr);
        char buff[64];

        sprintf(buff, "%.1f C", temperature);
        oled.drawStr(40, 17, buff);

        draw_graph(timestamp, temperature);
    }
    // air quality only
    else if (tvoc != UINT16_MAX)
    {
        oled.setFont(u8g2_font_ncenB10_tr);
        
        oled.drawStr(2, 12, "Air Quality:");

        oled.setFont(u8g2_font_ncenB12_tr);
        if (tvoc <= 65)
        {
            oled.drawStr(2, 40, "Good");
        }
        else if (tvoc <= 220)
        {
            oled.drawStr(2, 40, "Okay");
        }
        else if (tvoc <= 660)
        {
            oled.drawStr(2, 40, "Stuffy!");
        }
        else if (tvoc <= 2200)
        {
            oled.drawStr(2, 40, "Poor!");
        }
        else
        {
            oled.drawStr(2, 40, "Very Poor!");
        }
    }

    oled.nextPage();
}


void OLED::draw_graph(uint32_t timestamp, double temperature)
{
    // update only if necessary
    if (timestamp - last_update > graph_interval)
    {
        // move all items one up
        for (int i = curr_pop; i > -1; i--)
        {
            last_data[i+1] = last_data[i];
        }

        // insert new values
        last_data[0] = temperature;
    }

    // draw graph
    double max_val = d_max(last_data, curr_pop) + .1;
    double min_val = d_min(last_data, curr_pop) - .1;

    uint8_t last = 0;
    for (int i = 0; i < curr_pop; i++)
    {
        uint8_t x = GRAPH_X + (GRAPH_WIDTH - i);
        uint8_t y = GRAPH_Y + (d_map(last_data[i], min_val, max_val, GRAPH_HEIGHT, 0));

        oled.drawPixel(x, y);

        if (i != 0)
        {
            for (int dy = y; dy < last; dy++)
            {
                oled.drawPixel(x, dy);
            }
            for (int dy = y; dy > last; dy--)
            {
                oled.drawPixel(x, dy);
            }
        }

        last = y;
    }

    // Serial.printf("%d, %d\n", curr_pop, GRAPH_WIDTH);
    if (timestamp - last_update > graph_interval)
    {
        last_update = timestamp;
        if (curr_pop < GRAPH_WIDTH-2)
        {
            curr_pop++;
        }
    }
    oled.nextPage();
}


void OLED::wifi_connecting()
{
    oled.firstPage();
    do {
        oled.setFont(u8g2_font_ncenB10_tr);
        oled.drawStr(21, 28, "Connecting");
        oled.drawStr(36, 46, "to WiFi");
    } while (oled.nextPage());
}

void OLED::wifi_fail()
{
    oled.firstPage();
    do {
        oled.setFont(u8g2_font_ncenB10_tr);
        oled.drawStr(46, 28, "Failed");
        oled.drawStr(25, 46, "to Connect");
    } while (oled.nextPage());
}

void OLED::setting_up()
{
    oled.firstPage();
    do {
        oled.setFont(u8g2_font_ncenB14_tr);
        oled.drawStr(2, 18, "Setting up");
        oled.drawStr(13, 34, "Sensors");
        oled.nextPage();
    } while (oled.nextPage());
}
