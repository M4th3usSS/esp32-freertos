/**
 * Exemplo 04 - Passagem de parâmetros na criação da Task
 * Por: Matheus Sousa
 */

/* Biblioteca Arduino */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Mapeamento de pinos */
#define LED1 2
#define LED2 14

/* Macros */
#define BLINKER 200

/* Variáveis Globais */
uint32_t valor = 500;

/* Variáveis para armazenamento das Tasks */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;

/* Protótipos das Tasks */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );



// Task1 - Pisca LED
void vTask1( void *pvParameters )
{   
    uint32_t pin = ( uint32_t )pvParameters;
    pinMode(pin, OUTPUT);

    for( ;; )
    {
        digitalWrite( pin, HIGH );
        vTaskDelay( pdMS_TO_TICKS( BLINKER ) );
        digitalWrite( pin, LOW );
        vTaskDelay( pdMS_TO_TICKS( BLINKER ) );
    }
}

// Task2 - Impressão na Serial
void vTask2(void *pvParameters)
{
    uint32_t cont = (uint32_t)pvParameters;
    Serial.begin( 9600 );

    for( ;; ) 
    {
        Serial.println( "Task 2: " + String(cont++) );
        vTaskDelay( pdMS_TO_TICKS(BLINKER) );
    }
}



void setup()
{   
    xTaskCreate( vTask1, "TASK1", configMINIMAL_STACK_SIZE, (void*)LED1, 1, &xTask1Handle );
    xTaskCreate( vTask1, "TASK3", configMINIMAL_STACK_SIZE, (void*)LED2, 1, &xTask3Handle );
    xTaskCreate( vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, (void*)valor, 2, &xTask2Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
