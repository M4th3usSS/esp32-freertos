/************************************************************
 * Exemplo para demonstrar o uso de grupo de eventos
 * Por: Matheus Sousa 
************************************************************/

/* Inclusão da Bilioteca Arduino */
#include <Arduino.h>

/* Inclusão das Biliotecas do FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define LED 2

#define xTASK1_FLAG (1 << 0) // 1
#define xTASK2_FLAG (1 << 1) // 10
#define TIMEOUT     (1 << 2) // 100

TaskHandle_t xTask1Handle, xTask2Handle;
TimerHandle_t xTimer1Handle;
EventGroupHandle_t xEvents1Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void callBackTimer1(TimerHandle_t pxTimer);

void setup(void)
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    xEvents1Handle = xEventGroupCreate();
    xTimer1Handle = xTimerCreate("TIMER1", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer1);
    xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);
    xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &xTask2Handle);

    xTimerStart(xTimer1Handle, 0);
}

void loop(void)
{
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(500);
}

void vTask1(void *pvParameters)
{
    EventBits_t xBitis;

    for(;;) 
    {
        xBitis = xEventGroupWaitBits(xEvents1Handle, xTASK1_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
        Serial.println("Task 1 saiu do estado de bloqueio " + String(xBitis));
    }
}

void vTask2(void *pvParameters)
{
    EventBits_t xBitis;

    for(;;) 
    {
        xBitis = xEventGroupWaitBits(xEvents1Handle, xTASK2_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
        Serial.println("Task 1 saiu do estado de bloqueio " + String(xBitis));
    }
}

void callBackTimer1(TimerHandle_t pxTimer)
{
    static uint16_t xTemp;
    xTemp++;

    if (xTemp == 5) {
        xEventGroupSetBits(xEvents1Handle, xTASK1_FLAG);
    }
    else if(xTemp == 10) {
        xEventGroupSetBits(xEvents1Handle, xTASK2_FLAG);
    }
    else if(xTemp == 15) {
        xEventGroupSetBits(xEvents1Handle, xTASK1_FLAG | xTASK2_FLAG );
        xTemp = 0;
    }
}