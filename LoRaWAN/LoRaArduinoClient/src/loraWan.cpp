//                               //
// CONFIG YOUR DATA IN THIS FILE //
//             | |               //

/*******************************************************************************
 * *********************************************************************
 * * Using: MCCI LoRaWAN LMIC library V4.0.0                           *
 * * The following lines are the used example text, read it carefully.  *
 * *********************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!

 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in
 * arduino-lmic/project_config/lmic_project_config.h or from your BOARDS.txt.
 *
 *******************************************************************************/

#include "loraWan.h"

/************************************************/
/*********  THE THINGS NETWORK EXAMPLE  *********/
/*  (Comment it if you want to use Chirpstack)  */
/************************************************/

/* This APP_EUI must be in little-endian format (LSB), so least-significant-byte
 first. When copying an EUI from ttnctl output, this means to reverse
 the bytes. In this version, APP_EUI can be = 0, but you can config in TTS console. */
//static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* DEV_UI is configured in TTS console, should also be in little endian format (LSB), see above. (Warning: Must be in UPPERcase letters) */
//static const u1_t PROGMEM DEVEUI[8] = {0x7B, 0x1B, 0xDE, 0x2C, 0x7B, 0x4B, 0x18, 0x5F}; //Original (in MSB) was: 0x5F, 0x18, 0x4B, 0x7B, 0x2C, 0xDE, 0x1B, 0x7B

/* This key should be in big endian format (MSB), can be copied as-is.
 Also configured in TTS console  (Warning: Must be in UPPERcase letters) */
//static const u1_t PROGMEM APPKEY[16] = {0xBC, 0x21, 0x54, 0x80, 0xC2, 0xD7, 0x90, 0xF2, 0xC7, 0xCB, 0xB2, 0x88, 0xC7, 0x55, 0x33, 0xE7};

/************************************************/
/***********    CHIRPSTACK EXAMPLE    ***********/
/*      (Comment it if you want to use TTS)     */
/************************************************/

/* This APP_EUI must be in little-endian format (LSB), so least-significant-byte
 first. Chirpstack dont have APP_EUI, so write 0s. */
static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* DEV_UI is configured in TTS console, should also be in little endian format (LSB), see above. (Warning: Must be in LOWERcase letters) */
static const u1_t PROGMEM DEVEUI[8] = {0x7b, 0x1b, 0xde, 0x2c, 0x7b, 0x4b, 0x18, 0x5f}; // Original (in MSB) was: 0x5f, 0x18, 0x4b, 0x7b, 0x2c, 0xde, 0x1b, 0x7b

/* This key should be in big endian format (MSB), can be copied as-is.
 Also configured in Chirpstack console (Warning: Must be in LOWERcase letters) */
static const u1_t PROGMEM APPKEY[16] = {0xbc, 0x21, 0x54, 0x80, 0xc2, 0xd7, 0x90, 0xf2, 0xc7, 0xcb, 0xb2, 0x88, 0xc7, 0x55, 0x33, 0xe7};

/******* Lora pinmap config (change in hal.h) *******/

const lmic_pinmap lmic_pins = {
    .nss = LORA_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LORA_RST,
    .dio = {LORA_DIO0, LORA_DIO1, LORA_DIO2},
};

int channel = 0; //Use if you want to use only one Band's Channel

/****************** End of config ****************/
bool isLoopRunning = false;

void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

// static uint8_t payload[5]; //Data to send
static uint8_t mydata[] = "Hello World!";
static osjob_t sendjob;
String freq;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60; // The time betweend packet deliveries

/**** FUNCTIONS AND EVENTS ****/
void printHex2(unsigned v)
{
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}

void onEvent(ev_t ev)
{
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev)
    {
    case EV_SCAN_TIMEOUT:
        Serial.println(F("EV_SCAN_TIMEOUT"));
        break;
    case EV_BEACON_FOUND:
        Serial.println(F("EV_BEACON_FOUND"));
        break;
    case EV_BEACON_MISSED:
        Serial.println(F("EV_BEACON_MISSED"));
        break;
    case EV_BEACON_TRACKED:
        Serial.println(F("EV_BEACON_TRACKED"));
        break;
    case EV_JOINING:
        printLoraJoinInfoInDisplay();
        Serial.println(F("EV_JOINING"));
        break;
    case EV_JOINED:
        Serial.println(F("EV_JOINED"));
        {
            u4_t netid = 0;
            devaddr_t devaddr = 0;
            u1_t nwkKey[16];
            u1_t artKey[16];
            LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
            Serial.print("netid: ");
            Serial.println(netid, DEC);
            Serial.print("devaddr: ");
            Serial.println(devaddr, HEX);
            Serial.print("AppSKey: ");
            for (size_t i = 0; i < sizeof(artKey); ++i)
            {
                if (i != 0)
                    Serial.print("-");
                printHex2(artKey[i]);
            }
            Serial.println("");
            Serial.print("NwkSKey: ");
            for (size_t i = 0; i < sizeof(nwkKey); ++i)
            {
                if (i != 0)
                    Serial.print("-");
                printHex2(nwkKey[i]);
            }

            printLoraSuccesfullyJoinInDisplay();
            Serial.println();
        }
        // Disable link check validation (automatically enabled
        // during join, but because slow data rates change max TX
        // size, we don't use it in this example.
        LMIC_setLinkCheckMode(0);
        break;
    /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
    case EV_JOIN_FAILED:
        printLoraJoinErrorInDisplay();
        Serial.println(F("EV_JOIN_FAILED"));
        break;
    case EV_REJOIN_FAILED:
        printLoraJoinErrorInDisplay();
        Serial.println(F("EV_REJOIN_FAILED"));
        break;
    case EV_TXCOMPLETE:
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        if (LMIC.txrxFlags & TXRX_ACK)
            Serial.println(F("Received ack"));
        if (LMIC.dataLen)
        {
            Serial.print(F("Received "));
            Serial.print(LMIC.dataLen);
            Serial.println(F(" bytes of payload"));
        }
        // Schedule next transmission
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
        break;
    case EV_LOST_TSYNC:
        Serial.println(F("EV_LOST_TSYNC"));
        break;
    case EV_RESET:
        Serial.println(F("EV_RESET"));
        break;
    case EV_RXCOMPLETE:
        // data received in ping slot
        Serial.println(F("EV_RXCOMPLETE"));
        break;
    case EV_LINK_DEAD:
        Serial.println(F("EV_LINK_DEAD"));
        break;
    case EV_LINK_ALIVE:
        Serial.println(F("EV_LINK_ALIVE"));
        break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
    case EV_TXSTART:
        Serial.println(F("EV_TXSTART"));
        break;
    case EV_TXCANCELED:
        Serial.println(F("EV_TXCANCELED"));
        break;
    case EV_RXSTART:
        /* do not print anything -- it wrecks timing */
        break;
    case EV_JOIN_TXCOMPLETE:
        printLoraJoinErrorInDisplay();
        Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
        break;

    default:
        Serial.print(F("Unknown event: "));
        Serial.println((unsigned)ev);
        break;
    }
}

void LoraWan_startJob()
{
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // THE FOLLOWING LINE IS IMPORTANT, NOT INCLUDE IN EXAMPLE: Fix Fail request join error.
    LMIC_setClockError(10 * MAX_CLOCK_ERROR / 100);
    //

    //Warning: Use with caution
    //disableChannels(channel);
    
    do_send(&sendjob);
    isLoopRunning = true;
}

void disableChannels(int selectedChannel){
    // Define the single channel and data rate (SF) to use
    int dr = DR_SF7;

    // Disable all channels, except for the one defined above.
    // FOR TESTING ONLY!
    for (int i = 0; i < 9; i++)
    { // For EU; for US use i<71
        if (i != selectedChannel)
        {
            LMIC_disableChannel(i);
        }
    }

    // Set data rate (SF) and transmit power for uplink
    LMIC_setDrTxpow(dr, 14);
}

// This function is the LoRaWAN data loop: Read data, encrypt and save it to send.
void do_send(osjob_t *j)
{
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND)
    {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else
    {
        // First, read temperature and humidity
        am2315_readedData data = readAM2315Data();
        /*data.temp = data.temp / 100; // adjust for the f2sflt16 range (-1 to 1)
        data.hum = data.hum / 100;
        uint16_t payloadTemp = LMIC_f2sflt16(data.temp);
        uint16_t payloadHum = LMIC_f2sflt16(data.hum);

        byte tempLow = lowByte(payloadTemp);
        byte tempHigh = highByte(payloadTemp);
        byte humidLow = lowByte(payloadHum);
        byte humidHigh = highByte(payloadHum);

        payload[0] = tempLow;
        payload[1] = tempHigh;
        payload[2] = humidLow;
        payload[3] = humidHigh;
        LMIC_setTxData2(1, payload, sizeof(payload) - 1, 0);*/
        freq = String(LMIC.freq);
        LMIC_setTxData2(1, mydata, sizeof(mydata) - 1, 0);
        Serial.println(F("Packet queued"));

        if (isLoopRunning)
        {
            Serial.println("freq =" + freq);
            printSensorInfoInDisplay(24, 50);
            // Prepare upstream data transmission at the next possible time.
            printLoraSentInDisplay(freq);
        }
    }

    // Next TX is scheduled after TX_COMPLETE event.
}
