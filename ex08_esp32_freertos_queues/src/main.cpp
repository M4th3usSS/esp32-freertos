/***********************************************************
 * Exemplo que demonstra como criar uma fila, adicionar e ler
 * dados da fila através de duas tasks
 * Por: Matheus Sousa
 ************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include <FreeRTOSConfig.h>
#include <freertos/task.h>
#include <freertos/queue.h>

/* Macros */
#define LED 2

/* Definições */
QueueHandle_t xQueue1Handle;

TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

BaseType_t xReturnedTask1;
BaseType_t xReturnedTask2;

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
    pinMode(LED, OUTPUT);

    /* Cria Queue: */
    xQueue1Handle = xQueueCreate(5, sizeof(int32_t));

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

    // Cria Task 2:
    xReturnedTask2 = xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask2Handle);

    if (xReturnedTask2 == pdFAIL)
    {
        Serial.print("Não foi possível criar a Task: ");
        Serial.println(pcTaskGetName(xTask2Handle));
        while (1)
            ;
    }
    else
    {
        Serial.print("Task criada: ");
        Serial.println(pcTaskGetName(xTask2Handle));
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
 * Task 1
 *
 * Envia o valor de um contador para a queue
 */
void vTask1(void *pvParameters)
{

    int32_t lCounter = 0;

    for (;;)
    {
        if (lCounter < 10)
        {
            xQueueSend(xQueue1Handle, &lCounter, portMAX_DELAY);
            lCounter++;
        }
        else
        {
            lCounter = 0;
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * Task 2
 *
 * Consume valores da queue e os imprime na serial
 */
void vTask2(void *pvParameters)
{

    int32_t lReceivedValue = 0;

    for (;;)
    {
        if (xQueueReceive(xQueue1Handle, &lReceivedValue, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            Serial.println("Valor Recebido: " + String(lReceivedValue));
        }
        else
        {
            Serial.println("TIMEOUT");
        }
    }
}
