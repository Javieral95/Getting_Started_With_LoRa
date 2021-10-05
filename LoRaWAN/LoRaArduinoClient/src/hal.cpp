/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 */

#include "hal.h"

/** Objects: **/
Adafruit_AM2315 am2315;

/** Functions **/

void initAM2315()
{
    if (am2315.begin())
        Serial.println("Sensor AM2315 init succesfully!");
    else
        Serial.println("Sensor AM2315 not found, check wiring & pullups!");
}

am2315_readedData readAM2315Data()
{
    float temperature = -1000;
    float humidity = -1000;
    am2315_readedData ret;

    if (!am2315.readTemperatureAndHumidity(&temperature, &humidity))
        Serial.println("Failed to read data from AM2315 or don't have sensor!, send -1000 in both values");

    ret.temp = temperature;
    ret.hum = humidity;
    return ret;
}

void initSerialMonitor()
{
    Serial.begin(MONITOR_SPEED);
}
