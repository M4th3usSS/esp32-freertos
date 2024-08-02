/************************************************************
 * Exemplo que como enviar valores para uma fila
 * a partir de uma ISR
 * Por: Matheus Sousa
 ************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Macros */
#define LED 2
#define BUTTON_ISR 12

/* Definições */
QueueHandle_t xQueue1Handle;
TaskHandle_t xTask1Handle;
BaseType_t xReturnedTask1;

/* Protótipos */
void vCallBackISR(void);
void vTask1(void *pvParameters);

/**
 * Task Setup
 *
 */
void setup(void)
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(BUTTON_ISR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_ISR), vCallBackISR, RISING);

    /* Cria Queue: */
    xQueue1Handle = xQueueCreate(255, sizeof(int32_t));

    if (xQueue1Handle == NULL)
    {
        Serial.println("Não foi possível criar a Queue! ");
        while (1)
            ;
    }
    else
    {
        Serial.println("Queue criada!");
    }

    /* Cria Task 1  */
    xReturnedTask1 = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);

    if (xReturnedTask1 == pdFAIL)
    {
        Serial.print("Não foi possível criar a Task: ");
        Serial.println(pcTaskGetName(xTask1Handle));
        while (1)
            ;
    }
    else
    {
        Serial.print("Task criada: ");
        Serial.println(pcTaskGetName(xTask1Handle));
    }
}

/**
 * Task Loop
 *
 */
void loop(void)
{
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(1000));
}

/**
 * Função CallBack de ISR
 *
 */
void vCallBackISR(void)
{
    static uint32_t ulCounter = 0;
    ulCounter++;
    xQueueSendFromISR(xQueue1Handle, &ulCounter, NULL);
}

/**
 * Task 1
 *
 */
void vTask1(void *pvParameters)
{

    int32_t lReceivedValue;

    for (;;)
    {
        xQueueReceive(xQueue1Handle, &lReceivedValue, portMAX_DELAY);
        Serial.println("BUTTON_ISR pressionado: " + String(lReceivedValue));
    }
}
