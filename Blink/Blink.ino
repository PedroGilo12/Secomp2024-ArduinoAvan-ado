const int ledPin = 13; // Pino do LED
volatile bool ledState = false; // Estado do LED
char receivedChar; // Variável para armazenar o caractere recebido

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  pinMode(ledPin, OUTPUT); // Configura o pino do LED como saída
}

void loop() {
  // Verifica se o estado do LED foi alterado
  if (ledState) {
    digitalWrite(ledPin, HIGH); // Acende o LED
  } else {
    digitalWrite(ledPin, LOW); // Apaga o LED
  }
  
  // Aqui você pode adicionar outras lógicas de programa
}

// Esta função é chamada quando dados estão disponíveis na porta serial
void serialEvent() {
  while (Serial.available()) {
    receivedChar = Serial.read(); // Lê o caractere recebido
    
    // Verifica se o caractere é '1' ou '0'
    if (receivedChar == '1') {
      ledState = true; // Liga o LED
    } else if (receivedChar == '0') {
      ledState = false; // Desliga o LED
    }
  }
}
