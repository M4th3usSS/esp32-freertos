/************************************************************
* Exemplo 04 - Passagem de parâmetros na criação da Task
* Objetivo:
*      Passar parâmetros na criação de uma Task
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

/* Definições */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;

/* Protótipos */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


/**
 * Task Setup
 * 
 */
void setup(void)
{
    Serial.begin(9600);

    xTaskCreate(vTask1, "BLINK LED 1", configMINIMAL_STACK_SIZE, (void *)LED1, 1, &xTask1Handle);
    xTaskCreate(vTask1, "BLINK LED 2", configMINIMAL_STACK_SIZE, (void *)LED2, 1, &xTask3Handle);
    xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE + 1024, (void *)50, 2, &xTask2Handle);
}


/**
 * Task Loop
 * 
 */
void loop(void)
{
    vTaskDelay(pdMS_TO_TICKS(10000)); // Usada para liberar a CPU
}


/**
 * Task 1
 * 
 * Blink Led passado como parâmetro na criação da Task
 */
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


/**
 * Task 2
 * 
 * Contador com limite passado por parâmetro na criação da Task
 */
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
