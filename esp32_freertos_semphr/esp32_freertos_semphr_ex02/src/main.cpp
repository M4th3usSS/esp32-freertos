/*********************************************************************
* Exemplo para demonstrar o uso de semaforo binário dentro de uma ISR
* Por: Matheus Sousa
*********************************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Macros */
#define LED 2
#define BUTTON_ISR 12

/* Definições */
SemaphoreHandle_t xSemaphoreHandle1;

TaskHandle_t xTaskHandleISR;

/* Protótipos */
void vISRCallBack(void);
void vTaskISR(void *Parameters);


/**
 * Task Setup
 * 
 */
void setup()
{
	Serial.begin(9600);
	pinMode(LED, OUTPUT);
	pinMode(BUTTON_ISR, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_ISR), vISRCallBack, FALLING);

	xSemaphoreHandle1 = xSemaphoreCreateBinary();
	xTaskCreate(vTaskISR, "Task ISR", configMINIMAL_STACK_SIZE + 1024, NULL, 3, &xTaskHandleISR);
}


/**
 * Task Loop
 * 
 */
void loop()
{
	/* KeepAlive da aplicação */
	digitalWrite(LED, HIGH);
	vTaskDelay(pdMS_TO_TICKS(100));
	digitalWrite(LED, LOW);
	vTaskDelay(pdMS_TO_TICKS(1000));
}


/**
 * Função de CallBack da ISR
 * 
 */
void vISRCallBack(void)
{

	BaseType_t xHighPriorityTaskWoken = pdTRUE;

	xSemaphoreGiveFromISR(xSemaphoreHandle1, &xHighPriorityTaskWoken);

	if (xHighPriorityTaskWoken == pdTRUE)
	{
		portYIELD_FROM_ISR();
	}
}


/**
 * Task ISR
 * 
 */
void vTaskISR(void *Parameters)
{

	int32_t lCounter = 0;

	for (;;)
	{
		xSemaphoreTake(xSemaphoreHandle1, portMAX_DELAY);
		Serial.println("x: " + String(lCounter++));
	}
}
