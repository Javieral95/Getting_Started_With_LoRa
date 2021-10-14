/*
 * Header for the Hardware Abstraction Layer (HAL)
 * CTIC - LoRa MAC Test
 * Components ports
 */

#ifndef HAL_H
#define HAL_H

/* - - Includes - - */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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
#define LORA_SCK 5
#define LORA_MOSI 27
#define LORA_MOS0 19
#define LORA_RST 14
#define LORA_IRQ 22 //Warning: Same as Sensor's SCL
#define LORA_NSS 18 //V_SPI_CLK, Warning: Same as CS
#define LORA_DIO0 26
#define LORA_DIO1 33
#define LORA_DIO2 32

//Display
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define INIT_DISPLAY_COLUMN 0
#define TITLE_DISPLAY_ROW 0
#define TEMP_DISPLAY_ROW 20
#define HUM_DISPLAY_ROW 30
#define LORA_DISPLAY_ROW 50

/* - - Functions - - */
void initLoraMACTransceiver();
void initLoraMACCommunication();
void initSerialMonitor();
void initTempSensor();
void initDisplay();
void printSensorInfoInDisplay(float,float);
void printLoraInfoInDisplay();
void clearDisplay();

#endif