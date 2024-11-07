/**
 * O Led01 deve piscar 4 vezes mais rápido que o led02;
 */

#define LED_01 11
#define LED_02 10
#define BUTTON 2

void setup()
{
  pinMode(LED_01, OUTPUT);
  pinMode(LED_02, OUTPUT);
  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
 
  // Configuração do TIMER1
  TCCR1A = 0;                //confira timer para operação normal
  TCCR1B = 0;                //limpa registrador
  TCNT1  = 0;                //zera temporizad
 
  OCR1A = 0x3D09;                                   // carrega registrador de comparação A: 16MHz/1024/1Hz = 15625 = 0X3D09
  TCCR1B |= (1 << WGM12) | (1<<CS10)|(1 << CS12);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= (1 << OCIE1A);                          // habilita interrupção por igualdade de comparação
}
 
 
void loop()
{
  delay(1);
}
 
ISR(TIMER1_COMPA_vect)          // interrupção por igualdade de comparação no TIMER1
{
  digitalWrite(LED_01, digitalRead(LED_01) ^ 1);   //inverte estado do LED
}
