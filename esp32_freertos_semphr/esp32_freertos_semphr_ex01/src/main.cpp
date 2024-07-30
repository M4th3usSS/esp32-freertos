/**************************************************
* Exemplo para demonstrar o uso de semaforo binário
* Por: Matheus Sousa
***************************************************/

/* Biblioteca Arduino Core */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Macros */
#define LED 2

/* Definições Auxiliares */
TaskHandle_t xTaskADCHandle;
SemaphoreHandle_t xSemaphoreHandle1;

/* Protótipos das Tasks */
void vTaskADC(void *pvParameters);

/* Definição das Tasks  */
void setup()
{
	Serial.begin(9600);
	pinMode(LED, OUTPUT);

	/* Criando um Semaphore */
	xSemaphoreHandle1 = xSemaphoreCreateBinary();

	if (xSemaphoreHandle1 == NULL)
	{
		Serial.println("Não foi possível cria o semaforo");
		while (1)
			;
	}

	/* Criando uma Task */
	xTaskCreate(vTaskADC, "Task ADC", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskADCHandle);
}

void loop()
{
	digitalWrite(LED, !digitalRead(LED));
	Serial.println("Valor do LED : " + String(digitalRead(LED)));
	vTaskDelay(pdMS_TO_TICKS(3000));
	xSemaphoreGive(xSemaphoreHandle1);
}

void vTaskADC(void *pvParameters)
{

	int32_t lAcdValue;

	for (;;)
	{
		xSemaphoreTake(xSemaphoreHandle1, portMAX_DELAY);
		lAcdValue = analogRead(34);
		Serial.println("Valor do ADC: " + String(lAcdValue));
		Serial.println();
	}
}
