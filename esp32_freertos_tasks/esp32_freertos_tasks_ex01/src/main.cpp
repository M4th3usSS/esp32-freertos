/************************************************************
* Exemplo 01 - Criação de Tasks no FreeRTOS
* Objetivo:
*      Criar uma Task para Blink Led
*      Criar uma Task para Imprimir no monitor serial
* Por: Matheus Sousa
************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Macros */
#define LED 2

/* Definições */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

/* Protótipos */
void vTaks1(void *pvParameters);
void vTask2(void *pvParameters);


/**
 * Task Setup
 * 
 */
void setup(void)
{
    Serial.begin(9600);

    xTaskCreate(vTaks1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle);
    xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE + 1024, NULL, 2, &xTask2Handle);
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
 * Blink Led
 */
void vTaks1(void *pvParameters)
{
    pinMode(LED, OUTPUT);

    for (;;)
    {
        digitalWrite(LED, !digitalRead(LED));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


/**
 * Task 2
 * 
 * Imprime um contador na Serial
 */
void vTask2(void *pvParameters)
{
    uint32_t ulCount = 0;

    for (;;)
    {
        Serial.println("Task 2: " + String(ulCount));
        ulCount++;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
