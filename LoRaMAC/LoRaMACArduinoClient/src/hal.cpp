/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa MAC Test
 */

#include "hal.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
String Display_Title = "LoRaMAC end-device.";

void initSerialMonitor()
{
    Serial.begin(MONITOR_SPEED);
}

void initTempSensor()
{
    //TO-DO: Init AM2315 sensor
}

void initDisplay()
{
    //reset OLED display via software
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    //initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
}

void printSensorInfoInDisplay(float temp, float hum)
{
    display.setCursor(INIT_DISPLAY_COLUMN, TITLE_DISPLAY_ROW);
    display.print(Display_Title);
    display.setCursor(INIT_DISPLAY_COLUMN, TEMP_DISPLAY_ROW);
    display.print(String("") + "Temp: " + temp);
    display.setCursor(INIT_DISPLAY_COLUMN, HUM_DISPLAY_ROW);
    display.print(String("") + "Hum: " + hum);
    display.display();
}

void printLoraInfoInDisplay()
{
    display.setCursor(INIT_DISPLAY_COLUMN, LORA_DISPLAY_ROW);
    display.print("Packet sent!");
    display.display();
}

void clearDisplay(){
    display.clearDisplay();
}
