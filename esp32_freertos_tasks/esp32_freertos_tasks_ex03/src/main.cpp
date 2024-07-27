/**
 * Exemplo 03 - Parar e Retomar uma Task no FreeRTOS
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
TaskHandle_t xTaks1Handle = NULL;
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
        
        //  Suspendendo a Task1...
        if ( cont == 50 ) {
            Serial.println( "Suspendendo a Task1..." );
            digitalWrite( LED, LOW );
            vTaskSuspend( xTaks1Handle );
        }

        // Retomando a Task1...
        else if ( cont == 100 ) {
            Serial.println( "Retomando a Task1..." );
            vTaskResume( xTaks1Handle );
            cont = 0;
        }

        vTaskDelay( pdMS_TO_TICKS(BLINKER) );
    }
}



void setup()
{
    xTaskCreate( vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTaks1Handle );
    xTaskCreate( vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &xTask2Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
