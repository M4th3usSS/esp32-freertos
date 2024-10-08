/************************************************************
* Exemplo 07 - Verificando o uso da Stack de uma Task
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

    // Criando Tasks em CPUs diferentes
    xTaskCreatePinnedToCore(vTask1, "TASK1", configMINIMAL_STACK_SIZE, (void *)LED1, 1, &xTask1Handle, APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTask1, "TASK3", configMINIMAL_STACK_SIZE, (void *)LED2, 1, &xTask3Handle, APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTask2, "TASK2", configMINIMAL_STACK_SIZE + 1024, (void *)500, 2, &xTask2Handle, PRO_CPU_NUM);
}


/**
 * Task Loop
 * 
 */
void loop(void)
{
    vTaskDelay(pdMS_TO_TICKS(10000));
}


/**
 * Task 1
 * 
 * Blink Led
 */
void vTask1(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    uint32_t ulGpio = (uint32_t)pvParameters;
    pinMode(ulGpio, OUTPUT);

    for (;;)
    {
        digitalWrite(ulGpio, !digitalRead(ulGpio));
        vTaskDelay(pdMS_TO_TICKS(30));

        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); // NULL retorna a Task Corrente
        Serial.print(pcTaskGetName(NULL));                   // NULL retorna a Task Corrente
        Serial.print(" - HWM : ");
        Serial.println(uxHighWaterMark);
    }
}


/**
 * Task 2
 * 
 * Contador
 */
void vTask2(void *pvParameters)
{
    UBaseType_t uxHighWaterMark;
    uint32_t cont = (uint32_t)pvParameters;

    for (;;)
    {
        Serial.println("Task 2: " + String(cont++));
        vTaskDelay(pdMS_TO_TICKS(200));

        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); // NULL retorna a Task Corrente
        Serial.print(pcTaskGetName(NULL));                   // NULL retorna a Task Corrente
        Serial.print(" - HWM : ");
        Serial.println(uxHighWaterMark);
    }
}