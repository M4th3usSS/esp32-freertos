/**
 * Exemplo para demostrar o uso de task Notification de uma ISR
 * para uma task
 * Por: Matheus Sousa
*/

/* Inclusão da Biblioteca Arduino */
#include <Arduino.h>

/* Inclusão das Bibliotecas do FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 2
#define BUTTON_ISR 12

TaskHandle_t xTaskTrataISRHandle;

void vTaskTrataButton(void *Parameters);
void callBackISR(void);

void setup(void) 
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(BUTTON_ISR,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_ISR), callBackISR, FALLING);

    xTaskCreate(vTaskTrataButton, "Task Button", configMINIMAL_STACK_SIZE, NULL, 1, &xTaskTrataISRHandle);
}

void loop(void)  {
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void callBackISR(void) {
    vTaskNotifyGiveFromISR(xTaskTrataISRHandle, pdFALSE);
}

void vTaskTrataButton(void *Parameters)
{
    static uint32_t ilCounter = 0;

    for (;;)
    {
        ilCounter = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

        Serial.println("Tratando ISR do botão: " + String(ilCounter));
        digitalWrite(LED, !digitalRead(LED));
        delay(2000);
    }
}

