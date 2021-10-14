/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa WAN Test
 * LoRa WAN functions
 */

#include "hal.h"

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#define ARDUINO_LMIC_CFG_SUBBAND -1

/* DEFINES, don't worry nobody used this values */
//#define LORAWAN_FRECUENCY 868E6
//#define LORA_SYNC_WORD 0xF3

/* FUNCTIONS */
void printHex2(unsigned);
void onEvent(ev_t ev);
void LoraWan_startJob();
void do_send(osjob_t *);