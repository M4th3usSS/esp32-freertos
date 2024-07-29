/**
 * Exemplo 02 - Deletar Tasks no FreeRTOS
 * Objetivo: 
 *      Usar uma Task para deletar outra Task.
 *      Fazer uma Task deletar a si mesmo.
 * Por: Matheus Sousa
*/

/* 
    Biblioteca Arduino Core
*/
#include <Arduino.h>

/* 
    Bibliotecas FreeRTOS 
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* 
    Macros 
*/
#define LED 2

/* 
    Definições Globais
*/
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

/* 
    Protótipos das Tasks 
*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* 
    Definição das Tasks 
*/
void vTask1( void *pvParameters )
{
    pinMode(LED, OUTPUT);

    for( ;; )
    {
        digitalWrite( LED, !digitalRead(LED) );
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}

void vTask2(void *pvParameters)
{
    uint32_t ulCount = 0;

    for( ;; ) 
    {
        Serial.println( "Task 2: " + String(ulCount) );
        
        // Deletando a Task1
        if ( ulCount >= 50 ) {
            if ( xTask1Handle != NULL ) {
                Serial.println( "Deletando a Task1" );
                vTaskDelete( xTask1Handle );
                digitalWrite( LED, LOW ); // Garante LED LOW
                xTask1Handle = NULL;
            }
        }

        // Deletando a Task2
        if ( ulCount >= 100 ) {
            if ( xTask2Handle != NULL ) {
                Serial.println( "Deletando a Task2" );
                vTaskDelete( xTask2Handle );
                xTask2Handle = NULL;
            }
        }

        ulCount++;
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}

void setup()
{
    Serial.begin( 9600 );

    xTaskCreate( vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle );
    xTaskCreate( vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &xTask2Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS( 10000 ) );
}
