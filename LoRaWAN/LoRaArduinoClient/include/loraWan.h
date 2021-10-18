/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa WAN Test
 * LoRa WAN functions
 */

/* INCLUDES */
#include "hal.h"

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <LoraEncoder.h>

/* DEFINES, don't worry nobody used this values */
#define ARDUINO_LMIC_CFG_SUBBAND -1 //-1 -> Use all channels in band (There is no guarantee this will work)
//#define LORAWAN_FRECUENCY 868E6
//#define LORA_SYNC_WORD 0xF3

/* FUNCTIONS */
void printHex2(unsigned);
void onEvent(ev_t ev);
void LoraWan_startJob();
void disableChannels(int, int);
void do_send(osjob_t *);