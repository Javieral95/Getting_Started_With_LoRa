/*
 * Header for the Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 * Components ports
 */

#ifndef HAL_H 
#define HAL_H

/* - - Includes - - */
#include <Arduino.h>

#include <Time.h>
#include <Wire.h>
#include <Adafruit_AM2315.h> //Sensor
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* - - Defines - - */
#define MONITOR_SPEED 115200

//AM2315 Sensor, Warning: need two resistances from SDA and SCL to VIN (5V)
#define AM2315_SENSOR_SDA 21 //Yellow wire
#define AM2315_SENSOR_SCL 22 //White wire
struct am2315_readedData{
    float temp;
    float hum;
};

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

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels //(128)x64
#define SCREEN_HEIGHT 64 // OLED display height, in pixels //128x(64)

/* - - Functions - - */
void initSerialMonitor();
void initAM2315();
am2315_readedData readAM2315Data();

#endif