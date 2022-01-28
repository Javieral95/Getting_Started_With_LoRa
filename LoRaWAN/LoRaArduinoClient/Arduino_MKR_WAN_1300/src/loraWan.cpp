#include <loraWan.h>

// ################# Config: #################
// Copy from Chirpstack (App_Eui can be 0)

// Select your region (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band REGION = EU868;

const int SPREADING_FACTOR = 5; //5 for SF7, 6 for SF7(250Khz), 4 for SF8...
const int TIME_WAIT_FOR_REQUEST = 1000; // ms to wait for request after send data
// ############################################

LoRaModem modem(LORA_SERIAL);

void init_lora()
{
    Serial.println("->Initializing LoRa module");

    if (!modem.begin(REGION))
    {
        Serial.println("Failed to start module");
        while (1)
        {
        }
    };
    Serial.print("OK! Your device EUI is: ");
    Serial.println(modem.deviceEUI());
}

void connect_lora_otaa()
{
    Serial.println("->Trying connect to LoRa Network");
    int connected = modem.joinOTAA(APP_EUI, APP_KEY);
    if (!connected)
    {
        Serial.println("Something went wrong, try again.");
        while (1)
        {
        }
    }
    Serial.println("Successfully joined the network!");
}

void config_lora()
{
    Serial.println("->Enabling ADR and setting low spreading factor");
    modem.setADR(WAIT_FOR_RESPONSE);
    modem.dataRate(SPREADING_FACTOR);
}

void send_data(String data, bool waitForRequest = false)
{
    Serial.println("->Sending packet data");
    modem.beginPacket();
    modem.print(data);

    int err = modem.endPacket(waitForRequest);

    if (err > 0)
    {
        Serial.println("Ok!");
    }
    else
    {
        Serial.println("ERROR");
    }

    // Wait for response
    if (waitForRequest)
    {
        receive_response();
    }
}

void receive_response()
{
    Serial.println("->Waiting for response");
    delay(TIME_WAIT_FOR_REQUEST);

    if (!modem.available())
    {
        Serial.println("ERROR: No downlink message received at this time.");
        return;
    }
    char rcv[64];
    int i = 0;
    while (modem.available())
    {
        rcv[i++] = (char)modem.read();
    }
    Serial.print("Received: ");
    for (unsigned int j = 0; j < i; j++)
    {
        Serial.print(rcv[j] >> 4, HEX);
        Serial.print(rcv[j] & 0xF, HEX);
        Serial.print(" ");
    }
    Serial.println();
}
