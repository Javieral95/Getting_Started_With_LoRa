/*
 * Header for the Hardware Abstraction Layer (HAL)
 * CTIC - LoRa MAC Test
 * Components ports
 */

#ifndef HAL_H
#define HAL_H

/* - - Includes - - */
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

/* - - Defines - - */
#define MONITOR_SPEED 115200

//AM2315 Sensor, Warning: maybe need resistence
#define AM2315_SENSOR_SDA 21 //Yellow wire
#define AM2315_SENSOR_SCL 22 //White wire

//Lora PINs (See Pinout diagrams)
#define LORA_CS 18
#define LORA_RST 14
#define LORA_IRQ 22 //Warning: Same as Sensor's SCL
#define LORA_NSS 18 //V_SPI_CLK, Warning: Same as CS
#define LORA_DIO0 26
#define LORA_DIO1 33
#define LORA_DIO2 32

#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels //(128)x64
#define SCREEN_HEIGHT 64 // OLED display height, in pixels //128x(64)

/* - - Functions - - */
void initLoraMACTransceiver();
void initLoraMACCommunication();
void initSerialMonitor();
void initTempSensor();

#endif