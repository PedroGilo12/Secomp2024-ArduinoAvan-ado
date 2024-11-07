#define LED_01 11
#define BUTTON_PIN 3  /* Pino do botão */

unsigned long previousMillis = 0;          /* Armazena o último tempo em que o LED mudou de estado */
const long interval = 5000;                /* Intervalo de tempo para o LED mudar de estado (5 segundos) */
bool ledState = LOW;                       /* Estado atual do LED */
volatile bool buttonPressed = false;       /* Variável para indicar o estado do botão */
volatile unsigned long lastDebounceTime = 0; /* Armazena o último tempo em que o botão foi pressionado */
const long debounceDelay = 200;             /* Intervalo de debounce em milissegundos */

void setup() {
  pinMode(LED_01, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);       /* Configura o botão com resistor de pull-up interno */
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonToggle, FALLING);  /* Interrupção para mudança de estado */
}

void loop() {
  unsigned long currentMillis = millis();

  /* Verifica se o tempo decorrido é maior que o intervalo desejado */
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  /* Atualiza o tempo da última mudança */
    
    /* Alterna o estado do LED e armazena o estado atual */
    ledState = !ledState;
    digitalWrite(LED_01, ledState);
  }

  /* Verifica se o botão foi pressionado e altera o estado do LED */
  if (buttonPressed) {
    ledState = !ledState;           /* Alterna o estado do LED */
    digitalWrite(LED_01, ledState);
    buttonPressed = false;          /* Reseta o estado do botão */
  }
}

/* Função de interrupção para alternar o estado do botão */
void buttonToggle() {
  unsigned long currentMillis = millis();
  
  /* Verifica se o tempo decorrido desde a última leitura é maior que o debounceDelay */
  if (currentMillis - lastDebounceTime >= debounceDelay) {
    buttonPressed = true;           /* Define que o botão foi pressionado */
    lastDebounceTime = currentMillis; /* Atualiza o último tempo de debounce */
  }
}
