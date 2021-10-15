/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 */

#include "hal.h"

/** Objects: **/
Adafruit_AM2315 am2315;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
String Display_Title = "LoRaWAN OTAA Node.";

/** Functions **/

void initAM2315()
{
    if (am2315.begin())
        Serial.println("Sensor AM2315 init succesfully!");
    else
        Serial.println("Sensor AM2315 not found, check wiring & pullups!");
}

am2315_readedData readAM2315Data()
{
    float temperature = -1000;
    float humidity = -1000;
    am2315_readedData ret;

    if (!am2315.readTemperatureAndHumidity(&temperature, &humidity))
        Serial.println("Failed to read data from AM2315 or don't have sensor!, send -1000 in both values");

    ret.temp = temperature;
    ret.hum = humidity;
    return ret;
}

void initSerialMonitor()
{
    Serial.begin(MONITOR_SPEED);
}

//Display
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

    Serial.println("INIT DISPLAY");
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.display();
}

void printDisplayTitle()
{
    clearDisplay();
    display.setCursor(INIT_DISPLAY_COLUMN, INIT_DISPLAY_COLUMN);
    display.print(Display_Title);
    display.display();
}

void printSensorInfoInDisplay(float temp, float hum){
    printDisplayTitle();
    display.setCursor(INIT_DISPLAY_COLUMN, SECOND_DISPLAY_ROW);
    display.print(String("") + "Temp: " + temp);
    display.setCursor(INIT_DISPLAY_COLUMN, THIRD_DISPLAY_ROW);
    display.print(String("") + "Hum: " + hum);
    display.display();
}

void printLoraJoinInfoInDisplay()
{
    clearDisplay();
    printDisplayTitle();
    display.setCursor(INIT_DISPLAY_COLUMN, THIRD_DISPLAY_ROW);
    display.print("Trying to join . . .");
    display.display();
}

void printLoraJoinErrorInDisplay()
{
    display.setCursor(INIT_DISPLAY_COLUMN, FOURTH_DISPLAY_ROW);
    display.print("No JoinAccept, trying again");
    display.display();
}

void printLoraSuccesfullyJoinInDisplay()
{
    printDisplayTitle();
    display.setCursor(INIT_DISPLAY_COLUMN, SECOND_DISPLAY_ROW);
    display.print("Connected to LoRaWAN network!");
    display.display();
}

void printLoraSentInDisplay(String frecuency)
{    
    display.setCursor(INIT_DISPLAY_COLUMN, FOURTH_DISPLAY_ROW);
    display.print("(Packet sent! In:");
    display.setCursor(INIT_DISPLAY_COLUMN, FIFTH_DISPLAY_ROW);
    display.print(frecuency + " )");
    display.display();
}


void clearDisplay()
{
    display.clearDisplay();
    display.display();
}
