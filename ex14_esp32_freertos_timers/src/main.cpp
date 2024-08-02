/***************************************************
 * Exemplo para demonstrar o uso de software timer
 * Por: Matheus Sousa
 ***************************************************/

/* Bibliotaca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "FreeRTOSConfig.h"
#include "freertos/task.h"
#include "freertos/timers.h"

/* Macros */
#define LED1 2
#define LED2 14
#define BUTTON 12

/* Definições */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTimer1, xTimer2;

/* Protótipos */
void vTask1(void *Parameters);
void vCallBackTimer1(TimerHandle_t xTimer);
void vCallBackTimer2(TimerHandle_t xTimer);

void setup()
{
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    xTimer1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(1000), pdTRUE, 0, vCallBackTimer1);   // Timer com auto-reload
    xTimer2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(10000), pdFALSE, 0, vCallBackTimer2); // Timer sem auto-reload

    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE+1023, NULL, 1, &xTaskHandle1);

    xTimerStart(xTimer1, 0);
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}

/**
 * Task 1
 *
 * Tarefa que monitora o estado de um botão (com Deboucing) para parar o Timer 1 e iniciar o Timer 2
 */
void vTask1(void *Parameters)
{
    uint8_t ucDebouncingTime;

    for (;;)
    {
        if ((digitalRead(BUTTON) == LOW) && (xTimerIsTimerActive(xTimer2) == pdFALSE))
        {
            ucDebouncingTime++;

            if (ucDebouncingTime >= 10)
            {
                ucDebouncingTime = 0;
                
                xTimerStop(xTimer1, 0);
                xTimerStart(xTimer2, 0);

                digitalWrite(LED1, LOW);
                digitalWrite(LED2, HIGH);
                
                Serial.println("Parando Timer1 e iniciando o Timer2...");
            }
        }
        else
        {
            ucDebouncingTime = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * Observação:
 *
 * Não coloque muitas instruções nas funções de Callback dos timers pois funções grandes 
 * podem atrabalhar o funcionamento de outros timers.
 */


/**
 * Função call-back do Timer1
 *
 * Toggle LED1
 */
void vCallBackTimer1(TimerHandle_t xTimer)
{
    digitalWrite(LED1, !digitalRead(LED1));
}


/**
 * Função call-back do Timer2
 *
 * Desliga LED 2 e Retoma o Timer1
 */
void vCallBackTimer2(TimerHandle_t xTimer)
{
    xTimerStart(xTimer1, 0);
    digitalWrite(LED2, LOW);    
}