#include <Arduino.h>

#define pinVerde 12
#define pinAmarelo 11
#define pinVermelho 10
#define pinBotao 2

void botaoAcionado();

void botaoAcionado()
{

	static bool estado = LOW;
	static unsigned long delayEstado;

	if ((millis() - delayEstado) > 100) {
		estado = !estado;
		delayEstado = millis();
	}

	digitalWrite(pinVermelho, estado);
}

void setup()
{
	pinMode(pinVerde, OUTPUT);
	pinMode(pinAmarelo, OUTPUT);
	pinMode(pinVermelho, OUTPUT);

	pinMode(pinBotao, INPUT_PULLUP);

	// Cria a interrupção
	attachInterrupt(digitalPinToInterrupt(pinBotao), botaoAcionado, RISING);
}

void loop()
{
	digitalWrite(pinVerde, HIGH);
	digitalWrite(pinAmarelo, LOW);
	delay(1000);

	digitalWrite(pinVerde, LOW);
	digitalWrite(pinAmarelo, HIGH);
	delay(1000);

	noInterrupts(); // Pausa às interrupções
	{
		// ... Código que será executado com às interrupções desligadas 
	}
	interrupts(); // Retorna às interrupções

	//detachInterrupt(digitalPinToInterrupt(pinBotao)); // Deletando a interrupção

}