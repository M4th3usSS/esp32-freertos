/**
 * Exemplo 05 - Passagem de structs na criação da Task
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

/* structs */
typedef struct { 
    int32_t pin; 
    int32_t delay;
} StructLed_t;

typedef struct
{
    const char *message;
    int16_t init_cont;
    int16_t ms_speed;
} StructPrintSpeed_t;


/* Variáveis para armazenamento das Tasks */
TaskHandle_t xTask1Hanlde = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;
TaskHandle_t xTask4Handle = NULL;

/* Protótipos das Tasks */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );



/* Task1 - Pisca LED */
void vTask1( void *pvParameters )
{      
    StructLed_t xLed  = *( StructLed_t* )pvParameters;
    
    pinMode(xLed.pin, OUTPUT);

    for( ;; )
    {
        digitalWrite( xLed.pin, HIGH );
        vTaskDelay( pdMS_TO_TICKS( xLed.delay ) );
        digitalWrite( xLed.pin, LOW );
        vTaskDelay( pdMS_TO_TICKS( xLed.delay ) );
    }
}

/* Task2 - Impressão na Serial */
void vTask2(void *pvParameters)
{
    StructPrintSpeed_t xPrintSpeed = *( StructPrintSpeed_t* )pvParameters;

    for( ;; ) 
    {
        Serial.print( xPrintSpeed.message);
        Serial.println( String(xPrintSpeed.init_cont++));
        vTaskDelay( pdMS_TO_TICKS( xPrintSpeed.ms_speed ) );
    }
}



void setup()
{  
    Serial.begin( 9600 );

    /* Iniciando struct de Task1: */
    StructLed_t xLed1 = {
        .pin = 2, 
        .delay = 1000
    };

    /* Iniciando struct de Task2: */
    StructLed_t xLed2 = {
        .pin = 14, 
        .delay = 200
    };

    /* Iniciando struct de Task3: */
    StructPrintSpeed_t xPrintSpeed1 = {
        .message = "Task3 - ",
        .init_cont = 0,
        .ms_speed = 1000
    };

    /* Iniciando struct de Task3: */
    StructPrintSpeed_t xPrintSpeed2 = {
        .message = "Task4 - ",
        .init_cont = 100,
        .ms_speed = 1000
    };

    xTaskCreate( vTask1, "Task1", configMINIMAL_STACK_SIZE, &xLed1, 1, &xTask1Hanlde );
    xTaskCreate( vTask1, "Task2", configMINIMAL_STACK_SIZE , &xLed2, 2, &xTask2Handle );
    xTaskCreate( vTask2, "Task3", configMINIMAL_STACK_SIZE+1024, &xPrintSpeed1, 3, &xTask3Handle );
    xTaskCreate( vTask2, "Task4", configMINIMAL_STACK_SIZE+1024, &xPrintSpeed2, 4, &xTask4Handle );
}

void loop() {
    vTaskDelay( pdMS_TO_TICKS(10000) );
}
