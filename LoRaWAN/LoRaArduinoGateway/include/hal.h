/*
 * Header for the Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 * Components ports
 */

#ifndef HAL_H 
#define HAL_H

/* - - Includes - - */
#include "Arduino.h"

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

/* - - Defines - - */
#define MONITOR_SPEED 115200

#define ONE_WIRE_SENSORS_BUS 3

#define LORAWAN_FRECUENCY 868E6
#define LORA_SYNC_WORD 0xF3
//Lora PINs (See Pinout diagrams)
#define LORA_CS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_DIO1 33
#define LORA_DIO2 32

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels //(128)x64
#define SCREEN_HEIGHT 64 // OLED display height, in pixels //128x(64)

/* - - Functions - - */


#endif