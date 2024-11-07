#define LED1_PIN 11
#define LED2_PIN 10
#define BUTTON_PIN 2

#define MAX_EVT_BUFFER_SIZE 20
#define MAX_SERIAL_RECEIVED 100

volatile bool ledState = LOW;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200;

typedef enum FSM_EVENT {
  FSM_EVENT_NONE,
  FSM_EVENT_BUTTON_INTERRUPT,
  FSM_EVENT_TIMER_INTERRUPT,
  FSM_EVENT_SERIAL,
  FSM_EVENT_TOTAL,
} FSM_EVENT_E;

char serial_received[MAX_SERIAL_RECEIVED] = {0};

static struct evt_buffer_t {
  uint8_t evts[MAX_EVT_BUFFER_SIZE];
  uint8_t pos_write;
  uint8_t pos_read;
} evt_buffer = {
  .evts = {0},
  .pos_write = 0,
  .pos_read = 0
};

void fsm_post_evt(uint8_t evt) {
  if ((evt_buffer.pos_write + 1) % MAX_EVT_BUFFER_SIZE == evt_buffer.pos_read) {
    return;
  }
  
  evt_buffer.evts[evt_buffer.pos_write] = evt;
  evt_buffer.pos_write = (evt_buffer.pos_write + 1) % MAX_EVT_BUFFER_SIZE;
}

uint8_t fsm_get_evt(void) {
  if (evt_buffer.pos_read == evt_buffer.pos_write) {
    return 0;
  }
  
  uint8_t evt = evt_buffer.evts[evt_buffer.pos_read];
  evt_buffer.pos_read = (evt_buffer.pos_read + 1) % MAX_EVT_BUFFER_SIZE;
  return evt;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), toggleLED, FALLING);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
 
  OCR1A = 0x3D09;
  TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {
  int serial_received = 0;
  
  switch(fsm_get_evt()) {
    case FSM_EVENT_BUTTON_INTERRUPT:
      digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
      break;
    case FSM_EVENT_TIMER_INTERRUPT:
      digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
      break;
    case FSM_EVENT_SERIAL:
      digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
    break;
    default:
      break;
  }
}

void toggleLED() {
  unsigned long currentInterruptTime = millis();

  if (currentInterruptTime - lastInterruptTime > debounceDelay) {
    fsm_post_evt(FSM_EVENT_BUTTON_INTERRUPT);
  }

  lastInterruptTime = currentInterruptTime;
}

ISR(TIMER1_COMPA_vect) {
  fsm_post_evt(FSM_EVENT_TIMER_INTERRUPT);
}

void serialEvent() {
  char serial_received = Serial.read(); 
  
  fsm_post_evt(FSM_EVENT_SERIAL);
}
