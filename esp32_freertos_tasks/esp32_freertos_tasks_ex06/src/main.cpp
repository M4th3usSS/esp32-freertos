/************************************************************
* Exemplo 06 - Criando tarefas em núcleos diferentes
* Objetivo:
*      Separar as Tasks em cores diferentes
* Por: Matheus Sousa
************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Macros */
#define LED1 2
#define LED2 14

/* Definições Auxiliares */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;

/* Protótipos das Tasks */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Definição das Tasks */
void setup()
{
    Serial.begin(9600);

    // Criando Tasks em CPUs diferentes
    xTaskCreatePinnedToCore(vTask1, "TASK1", configMINIMAL_STACK_SIZE, (void *)LED1, 1, &xTask1Handle, APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTask1, "TASK3", configMINIMAL_STACK_SIZE, (void *)LED2, 1, &xTask3Handle, APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTask2, "TASK2", configMINIMAL_STACK_SIZE + 1024, (void *)500, 2, &xTask2Handle, PRO_CPU_NUM);
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(10000));
}

void vTask1(void *pvParameters)
{
    uint32_t ulGpio = (uint32_t)pvParameters;

    pinMode(ulGpio, OUTPUT);

    for (;;)
    {
        digitalWrite(ulGpio, !digitalRead(ulGpio));
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void vTask2(void *pvParameters)
{
    uint32_t ulCount = 0;
    uint32_t ulCountMax = (uint32_t)pvParameters;

    for (;;)
    {
        Serial.println("Task 2: " + String(ulCount));
        ulCount++;

        if (ulCount >= ulCountMax)
            ulCount = 0;

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
