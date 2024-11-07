#include <SoftwareSerial.h>

typedef enum event {
  ENTRY_EVENT = 0,
  EXIT_EVENT,
  SEND_COMMAND,
  ESP8266_SERIAL_RECEIVED,
} event_t;

void (*current_state)(event_t event);

SoftwareSerial esp8266(10, 11);  // RX, TX
String readString;
String writeString;

bool awaitingCIPSEND = false; // Variável para monitorar quando o comando AT+CIPSEND é enviado
String webpage;               // Armazena a página a ser enviada
int linkID;                   // ID de link para enviar a página

void setup() {
  Serial.begin(115200);
  esp8266.begin(4800);
  pinMode(13, OUTPUT);

  current_state = connect_state;
  send_event(ENTRY_EVENT);
}

void send_event(event_t event) {
  current_state(event);
}

struct {
  String commands[6];
  int index;
} connect_process = {
  .commands = {
    "AT",
    "AT+CWMODE=1",
    "AT+CWJAP=\"PHVG_2.4G\",\"14521011\"",
    "AT+CIFSR",
    "AT+CIPMUX=1",
    "AT+CIPSERVER=1,80",
  },
  .index = 0,
};

void connect_state(event_t event) {
  switch (event) {
    case ENTRY_EVENT:
      send_event(SEND_COMMAND);
      break;

    case SEND_COMMAND:
      if (connect_process.index < 6) {
        esp8266.println(connect_process.commands[connect_process.index]);
        connect_process.index++;
      } else {
        Serial.println("Processo concluido");
        current_state = server_state;
        current_state(ENTRY_EVENT);
      }
      break;

    case ESP8266_SERIAL_RECEIVED:
      Serial.println(readString);
      if (readString == "OK\r\n") {
        Serial.println("Recebeu ok");
        send_event(SEND_COMMAND);
      }
      break;
  }
}

void server_state(event_t event) {
  switch (event) {
    case ENTRY_EVENT:
      Serial.println("Entrou no estado de servidor");
      break;

    case ESP8266_SERIAL_RECEIVED:
      Serial.println(readString);

      if (readString.indexOf("LED") != -1) {
        digitalWrite(13, !digitalRead(13));
        esp8266.println("AT+CIPCLOSE=0");
      }

      if (readString.indexOf("CONNECT") != -1) {
        webpage = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        webpage += "<!DOCTYPE HTML><html><body>";
        webpage += "<h1>Controle do LED</h1>";
        webpage += "<a href=\"/LED\"><button>Alternar LED</button></a><br>";
        webpage += "</body></html>";

        linkID = readString[0] - '0';
        esp8266.println("AT+CIPSEND=" + String(linkID) + "," + String(webpage.length()));
        awaitingCIPSEND = true; // Marcamos que estamos aguardando a resposta do CIPSEND
      }

      if (awaitingCIPSEND && readString.indexOf("OK") != -1) {
        // Envia a página após confirmação do CIPSEND
        esp8266.print(webpage);
        awaitingCIPSEND = false;
      }
      break;
  }
}

void loop() {
  if (esp8266.available()) {
    char c = esp8266.read();
    readString += c;

    if (c == '\n') {
      send_event(ESP8266_SERIAL_RECEIVED);
      readString = "";
    }
  }

  if (Serial.available()) {
    char c = Serial.read();
    writeString += c;

    if (c == '\n') {
      esp8266.print(writeString);
      writeString = "";
    }
  }
}
