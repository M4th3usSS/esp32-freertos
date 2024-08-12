#include <Arduino.h>

//#define ARDUINO_FUNC  1  //comente para uso de macros

#define setBit(valor,bit)     (valor |= (1<<bit))
#define clearBit(valor,bit)   (valor &= ~(1<<bit))
#define testBit(valor,bit)    (valor & (1<<bit))
#define toggleBit(valor, bit)  (valor ^= (1<<bit))


#ifdef ARDUINO_FUNC
#define LED 13          //pino 13 para o LED
#define BT  2           //pino 2 para o BT
#else
#define LED 5           //pino PB5 para o LED
#define BT  2           //pino PD2 para o BT
#endif


void setup() {
  #ifndef ARDUINO_FUNC
  setBit(DDRB,LED);    //configura pino PB5 como saída
  clearBit(DDRD,BT);  //configura pino PD2 como entrada
  #else
  pinMode(LED, OUTPUT); 
  pinMode(BT, INPUT);
  #endif
  
}

void loop() {  
  
  while(1){
    
    toggleBit(PORTB,LED);
    delay(1000);
  }
  
  
  #ifndef ARDUINO_FUNC
  if(!testBit(PIND,BT)){   //se botão pressionado 
    setBit(PORTB,LED);      //liga led
  }
  else{                    //se não
    clearBit(PORTB,LED);    //apaga led
  }
  #else
  if(digitalRead(BT) == LOW){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }
  #endif
}