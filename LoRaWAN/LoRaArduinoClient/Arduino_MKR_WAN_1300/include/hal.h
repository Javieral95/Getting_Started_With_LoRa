/*
 * Header for the Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 * Components ports
 */

#ifndef HAL_H 
#define HAL_H

/* - - Includes - - */
#include <Arduino.h>

/* - - Defines - - */
#define MONITOR_SPEED 115200
#define LORA_SERIAL Serial1

/* - - Functions - - */
void initSerialMonitor();

#endif