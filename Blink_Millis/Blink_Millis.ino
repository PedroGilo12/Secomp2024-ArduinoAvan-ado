#define LED_01 11

unsigned long previousMillis = 0;  /* Armazena o último tempo em que o LED mudou de estado */
const long interval = 5000;        /* Intervalo de tempo para o LED mudar de estado (5 segundos) */

void setup() {
  pinMode(LED_01, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  /* Verifica se o tempo decorrido é maior que o intervalo desejado */
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  /* Atualiza o tempo da última mudança */

    /* Alterna o estado do LED */
    digitalWrite(LED_01, !digitalRead(LED_01));
  }
}
