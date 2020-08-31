#include <BatteryTask.h>
#include "shutdown.h"

BatteryTask::BatteryTask(uint32_t intervalMs) : interval(intervalMs) {}

void BatteryTask::timerCB(TimerHandle_t handle)
{
    ((BatteryTask *)pvTimerGetTimerID(handle))->batteryMeasureAndReport();
}

err_t BatteryTask::begin()
{
    BLEBas::begin();
    batteryMeasureAndReport();
    timer.begin(interval, timerCB, this);
    timer.start();
    return ERROR_NONE;
}

void BatteryTask::batteryMeasureAndReport()
{
    uint32_t battVoltage = analogRead(PIN_VBAT);
    uint8_t percentage = (battVoltage - 853) * 10 / 17;
    if (battVoltage < 853)
        shutdown();

#if CFG_DEBUG >= 2
    printf("Battery Measured: %lu, %%%d\n", battVoltage, percentage);
#endif

    static uint8_t notifiedVal = 0;
    if (abs(percentage - notifiedVal) > 5){
        notify(percentage);
        notifiedVal = percentage;
    }
    write(percentage);
}