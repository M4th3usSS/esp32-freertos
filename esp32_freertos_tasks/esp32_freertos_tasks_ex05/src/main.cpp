/**
 * Exemplo 05 - Passagem de structs na criação da Task
* Objetivo:
 *      Passar parâmetros em structs na criação de uma Task 
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
#define LED1 2
#define LED2 14

/* 
    Typedef structs
*/
typedef struct { 
    uint16_t usGpio;
    uint16_t usDelay_MS;
} StructLed_t;

typedef struct
{
    const char *message;
    uint16_t usCounter;
    uint16_t usCounterMax;
} StructCounter_t;

/* 
    Definições Auxiliares
*/
TaskHandle_t xTask1Hanlde = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;
TaskHandle_t xTask4Handle = NULL;

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
    StructLed_t xLed  = *( StructLed_t* )pvParameters;
    
    pinMode(xLed.usGpio, OUTPUT);

    for( ;; )
    {
        digitalWrite( xLed.usGpio, !digitalRead(xLed.usGpio) );
        vTaskDelay( pdMS_TO_TICKS( xLed.usDelay_MS ) );
    }
}

void vTask2(void *pvParameters)
{
    StructCounter_t xCounter = *( StructCounter_t* )pvParameters;

    for( ;; ) 
    {
        Serial.print( xCounter.message);
        Serial.print("[ ");
        Serial.print( String(xCounter.usCounter));
        Serial.print(" / ");
        Serial.print( String(xCounter.usCounterMax));
        Serial.println(" ]");

        if (xCounter.usCounter >= xCounter.usCounterMax)
            xCounter.usCounter = 0;

        xCounter.usCounter++;
        vTaskDelay( pdMS_TO_TICKS( 100 ) );
    }
}

void setup()
{  
    Serial.begin( 9600 );

    /* 
    Iniciando struct de Task1: 
    */
    StructLed_t xLed1 = { .usGpio = 2, .usDelay_MS = 1000 };

    /* 
        Iniciando struct de Task2: 
    */
    StructLed_t xLed2 = { .usGpio = 14, .usDelay_MS = 200 };

    /* 
        Iniciando struct de Task3: 
    */
    StructCounter_t xCounter1 = { .message = "Estou executando a Task 03: ", .usCounter = 0, .usCounterMax = 10 };

    /* 
        Iniciando struct de Task3: 
    */
    StructCounter_t xCounter2 = { .message = "Estou executando a Task 04: ", .usCounter = 10, .usCounterMax = 20 };

    xTaskCreate( vTask1, "Task1", configMINIMAL_STACK_SIZE, &xLed1, 1, &xTask1Hanlde );
    xTaskCreate( vTask1, "Task2", configMINIMAL_STACK_SIZE , &xLed2, 2, &xTask2Handle );
    xTaskCreate( vTask2, "Task3", configMINIMAL_STACK_SIZE+1024, &xCounter1, 3, &xTask3Handle );
    xTaskCreate( vTask2, "Task4", configMINIMAL_STACK_SIZE+1024, &xCounter2, 4, &xTask4Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
