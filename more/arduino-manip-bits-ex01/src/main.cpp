/**
 * Exemplo: Operadores bit-a-bit linguagem C
 * Source: https://embarcados.com.br/bits-em-linguagem-c/
 * 
 * O exemplo a seguir exibe o uso das macros apresentadas para manipulação de bits no Atmega328P. 
 */

#include <Arduino.h>

#define setBit(valor,bit)     (valor |= (1<<bit))
#define clearBit(valor,bit)   (valor &= ~(1<<bit))
#define testBit(valor,bit)    (valor & (1<<bit))
#define toggleBit(valor, bit)  (valor ^= (1<<bit))

#define LED 5           //pino PB5 para o LED
#define BT  2           //pino PD2 para o BT

void setup() {
  setBit(DDRB,LED);    //configura pino PB5 como saída
  clearBit(DDRD,BT);   //configura pino PD2 como entrada  
}

void loop() {  
  
  if(!testBit(PIND,BT)){   //se botão pressionado 
    setBit(PORTB,LED);     //liga led
  }
  else{                     //se não
    clearBit(PORTB,LED);    //apaga led
  }
  
}
