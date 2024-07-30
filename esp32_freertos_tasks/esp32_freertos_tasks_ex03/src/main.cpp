/************************************************************
* Exemplo 03 - Parar e Retomar uma Task no FreeRTOS
* Objetivo:
*      Usar uma Task para suspender e retomar a execução de outra Task
* Por: Matheus Sousa
************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Macros */
#define LED 2

/* Definições Auxiliares */
TaskHandle_t xTasks1Handle = NULL;
TaskHandle_t xTasks2Handle = NULL;

/* Protótipos das Tasks */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Definição das Tasks  */
void setup()
{
    Serial.begin(9600);

    xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTasks1Handle);
    xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &xTasks2Handle);
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(10000));
}

void vTask1(void *pvParameters)
{
    pinMode(LED, OUTPUT);

    for (;;)
    {
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    uint32_t ulCount = 0;

    for (;;)
    {
        Serial.println("Task 2: " + String(ulCount++));

        //  Suspendendo a Task1...
        if (ulCount == 50)
        {
            Serial.println("Suspendendo a Task1...");
            vTaskSuspend(xTasks1Handle);
            digitalWrite(LED, LOW);
        }

        // Retomando a Task1...
        else if (ulCount == 100)
        {
            Serial.println("Retomando a Task1...");
            vTaskResume(xTasks1Handle);
            ulCount = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
