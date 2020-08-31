#include <Arduino.h>
#include <bluefruit.h>

extern "C"
{
    extern int __register_exitproc(int, void (*)(void), void *, void *);
    void _exit(int)
    {
        systemOff(7, false);
        for (;;)
            ;
    }
}
volatile auto __register_exitproc_ = __register_exitproc;

TaskHandle_t tearDownTask;
void bluefruitDisconnectHandler(ble_evt_t* evt)
{
    if (evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED)
        xTaskNotify(tearDownTask, 0, eNoAction);
}

void shutdown()
{
    tearDownTask = xTaskGetCurrentTaskHandle();
    Bluefruit.setEventCallback(bluefruitDisconnectHandler);
    Bluefruit.disconnect(Bluefruit.connHandle());
    xTaskNotifyWait(0x00000000,0x00000000,nullptr, ms2tick(100));
    Bluefruit.setEventCallback(nullptr);
    exit(0);
}