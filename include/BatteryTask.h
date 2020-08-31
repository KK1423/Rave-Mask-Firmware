#ifndef BATTTASK_H
#define BATTTASK_H

#include <Arduino.h>
#include <bluefruit.h>

class BatteryTask : BLEBas
{
public:
    BatteryTask(uint32_t interval = 30000);
    err_t begin() override;

private:
    uint32_t interval;
    SoftwareTimer timer;
    static void timerCB(TimerHandle_t handle);
    void batteryMeasureAndReport();
};

#endif