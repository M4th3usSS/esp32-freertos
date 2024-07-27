/**
 * Exemplo 01 - Criação de Tasks no FreeRTOS
 * Por: Matheus Sousa
 */

/* Biblioteca Arduino */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Mapeamento de pinos */
#define LED 2

/* Variáveis para armazenamento das Tasks */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/* Protótipos das Tasks */
void vTaks1( void *pvParameters );
void vTask2( void *pvParameters );



/* Task1 - Pisca LED */
void vTaks1( void *pvParameters )
{
    pinMode( LED, OUTPUT );

    for( ;; )
    {
        digitalWrite( LED, HIGH );
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
        digitalWrite( LED, LOW );
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}

/* Task2 - Impressão na Serial */ 
void vTask2( void *pvParameters )
{
    Serial.begin( 9600 );

    uint32_t cont = 0;

    for( ;; )
    {
        Serial.println( "Task 2: " + String(cont++) );
        vTaskDelay( pdMS_TO_TICKS(200) );
    }
}



void setup()
{
    xTaskCreate( vTaks1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle );
    xTaskCreate( vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
