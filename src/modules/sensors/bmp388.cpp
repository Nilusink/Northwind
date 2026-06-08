#include "bmp388.hpp"


using namespace sensors;


BMP388::BMP388()
  : bmp()
{};


void BMP388::setup()
{
    int i = 0;
    // connect to sensor
    while (!bmp.begin_I2C())
    {
        Serial.println("Couldn't find BMP388");
        i++;
        delay(200);
    
        if (i >= 10)
        {
            while (1) delay(1);
        }
    }

    // configure sensor
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_32X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_127);
    bmp.setOutputDataRate(BMP3_ODR_0_1_HZ);

}

void BMP388::update(uint32_t timestamp)
{
    // read if necessary
    if (timestamp - last_update > read_interval)
    {
        uint8_t i = 0;
        while (!bmp.performReading() && i < 10)
        {
            delay(50);
            i++;
        }

        last_update = timestamp;
    }
}


double BMP388::get_temperature()
{
    return bmp.temperature;
}


double BMP388::get_pressure()
{
    return bmp.pressure / 100;  // hPa
}
