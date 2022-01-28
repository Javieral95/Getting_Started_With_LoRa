/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 */

#include "hal.h"


/** Functions **/

void initSerialMonitor()
{
    Serial.begin(MONITOR_SPEED);
    while (!Serial) ;
}
