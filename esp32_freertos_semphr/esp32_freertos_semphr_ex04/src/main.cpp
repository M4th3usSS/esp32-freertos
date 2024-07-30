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

/* Definições Auxiliares */
SemaphoreHandle_t xMutex;

TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/* Protótipos das Tasks */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/*
    Simulando um recurso exclusivo
    Uma Task não pode acessar essa função enquanto outra Task estiver há usando
*/ 
void vSendInformation( int32_t lInformation)
{
    xSemaphoreTake(xMutex, portMAX_DELAY);
    Serial.println("Enviando informação da Task: " + String(lInformation));
    delay(1000);
    xSemaphoreGive(xMutex);
}

/* Definição das Tasks  */
void setup()
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    xMutex = xSemaphoreCreateMutex();
    
    xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTaskHandle1);
    xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE+1024, NULL, 4, &xTaskHandle2);
}

void loop()
{
    /* KeepAlive da aplicação */
    digitalWrite( LED, HIGH );
    vTaskDelay( pdMS_TO_TICKS(100) );
    digitalWrite( LED, LOW );
    vTaskDelay( pdMS_TO_TICKS(1000) );
}

void vTask1(void *pvParameters)
{

    for ( ;; )
    {
        vSendInformation(1);
        vTaskDelay(10); // precisamos usar timers para liberar a CPU em tasks com maior prioridade
    }
}
void vTask2(void *pvParameters)
{

    for ( ;; )
    {
        vSendInformation(2);
        vTaskDelay(10); // precisamos usar timers para liberar a CPU em tasks com maior prioridade
    }
}
