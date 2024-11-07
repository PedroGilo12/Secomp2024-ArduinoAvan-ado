#define LED_01 11
#define BUTTON_PIN 7  /* Pino do botão */

unsigned long previousMillis = 0;  /* Armazena o último tempo em que o LED mudou de estado */
const long interval = 5000;        /* Intervalo de tempo para o LED mudar de estado (5 segundos) */

void setup() {
  pinMode(LED_01, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  /* Configura o botão com resistor de pull-up interno */
}

void loop() {
  /* Verifica se o botão está pressionado */
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_01, HIGH);  /* Liga o LED enquanto o botão está pressionado */
  } else {
    unsigned long currentMillis = millis();

    /* Verifica se o tempo decorrido é maior que o intervalo desejado */
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;  /* Atualiza o tempo da última mudança */

      /* Alterna o estado do LED */
      digitalWrite(LED_01, !digitalRead(LED_01));
    }
  }
}
