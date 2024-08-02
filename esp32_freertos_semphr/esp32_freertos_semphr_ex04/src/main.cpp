/***************************************************************************
 * Exemplo para demonstrar o uso de MUTEX para acesso a recursos e a inversão
 * de prioridade (arecursos restritos)
 * Por: Matheus Sousa
 ***************************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Macros */
#define LED 2

/* Definições */
SemaphoreHandle_t xMutex;

TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/* Protótipos */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/**
 * Task Setup
 * 
 */
void setup()
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    xMutex = xSemaphoreCreateMutex();
    
    xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTaskHandle1);
    xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE+1024, NULL, 4, &xTaskHandle2);
}


/**
 * Task Loop
 * 
 */
void loop()
{
    /* KeepAlive da aplicação */
    digitalWrite( LED, HIGH );
    vTaskDelay( pdMS_TO_TICKS(100) );
    digitalWrite( LED, LOW );
    vTaskDelay( pdMS_TO_TICKS(1000) );
}


/**
 * Task 1
 * 
 */
void vTask1(void *pvParameters)
{

    for ( ;; )
    {
        vSendInformation(1);
        vTaskDelay(10); // precisamos usar timers para liberar a CPU em tasks com maior prioridade
    }
}


/**
 * Task 2
 * 
 */
void vTask2(void *pvParameters)
{

    for ( ;; )
    {
        vSendInformation(2);
        vTaskDelay(10); // precisamos usar timers para liberar a CPU em tasks com maior prioridade
    }
}

/*
    Simulando uma função com recurso exclusivo
    Tasks não podem compartilhar instruções dessa função simultaneamente!
*/ 
void vSendInformation( int32_t lInformation)
{
    xSemaphoreTake(xMutex, portMAX_DELAY);
    Serial.println("Enviando informação da Task: " + String(lInformation));
    delay(1000);
    xSemaphoreGive(xMutex);
}