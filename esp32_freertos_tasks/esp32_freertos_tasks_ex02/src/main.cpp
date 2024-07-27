/**
 * Exemplo 02 - Deletar Tasks no FreeRTOS
 * Por: Matheus Sousa
 */

/* Biblioteca Arduino */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Mapeamento de pinos */
#define LED 2
#define BLINKER 200

/* Variáveis para armazenamento das Tasks */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

/* Protótipos das Tasks */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );



// Task1 - Pisca LED
void vTask1( void *pvParameters )
{
    pinMode(LED, OUTPUT);

    for( ;; )
    {
        digitalWrite( LED, HIGH );
        vTaskDelay( pdMS_TO_TICKS( BLINKER ) );
        digitalWrite( LED, LOW );
        vTaskDelay( pdMS_TO_TICKS( BLINKER ) );
    }
}

// Task2 - Impressão na Serial
void vTask2(void *pvParameters)
{
    Serial.begin( 9600 );

    uint32_t cont = 0;

    for( ;; ) 
    {
        Serial.println( "Task 2: " + String(cont++) );
        
        // Deletando a Task1
        if ( cont >= 50 ) {
            
            if ( xTask1Handle != NULL ) {
                Serial.println( "Deletando a Task1" );
                vTaskDelete( xTask1Handle );
                digitalWrite( LED, LOW ); // Garante LED LOW
                xTask1Handle = NULL;
            }
        }

        // Deletando a Task2
        if ( cont >= 100 ) {
            
            if ( xTask2Handle != NULL ) {
                Serial.println( "Deletando a Task2" );
                vTaskDelete( xTask2Handle );
                xTask2Handle = NULL;
            }
        }

        vTaskDelay( pdMS_TO_TICKS(BLINKER) );
    }
}



void setup()
{
    xTaskCreate( vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle );
    xTaskCreate( vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &xTask2Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
