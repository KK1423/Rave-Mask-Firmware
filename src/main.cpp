#include <Arduino.h>
#include <bluefruit.h>
#include <InternalFileSystem.h>
#include <Adafruit_IS31FL3731.h>

#include <BatteryTask.h>

void startAdv()
{
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);
}

BLEDfu DFUService;
BLEBms BMSService;
BatteryTask batteryTask;
Adafruit_IS31FL3731 panel;

void setup()
{
    Bluefruit.begin();
    Bluefruit.setTxPower(4);
    Bluefruit.setName("Rave Mask");
    Bluefruit.Security.setPIN("494246");

    DFUService.begin();
    BMSService.begin();
    batteryTask.begin();

    startAdv();
    delay(1000);

    while (!panel.begin())
        ;
}

void loop()
{
    Adafruit_IS31FL3731::Frame buffer;
    static float xPos = -1;
    static float yPos = 0;
    static float xVel = 1;
    static float yVel = 1;

    if (xPos >= 7 || xPos <= 0)
    {
        xVel *= -1;
        xPos = signbit(xVel) ? 6.9 : 0.1;
    }
    else if (yPos >= 8 || yPos <= 0)
    {
        yVel *= -1;
        yPos = signbit(yVel) ? 7.9 : 0.1;
        panel.resetOn();
    }
    else
    {
        xVel += ((rand() % 51) - 25) * 0.3;
        yVel += ((rand() % 51) - 25) * 0.3;
        xVel *= 0.97;
        yVel *= 0.97;
        xPos += xVel * 0.01;
        yPos += yVel * 0.01;
    }

    for (uint8_t x = 0; x < 8; x++)
        for (uint8_t y = 0; y < 9; y++)
        {
            float dist = (xPos - x) * (xPos - x) + (yPos - y) * (yPos - y);
            uint8_t val = 255.0 * exp(-sqrt(dist));

            buffer.pixel(x,y) = val;
        }
    panel.blitFrame(buffer);
    delay(10);
}
