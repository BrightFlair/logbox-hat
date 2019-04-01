#include <EnableInterrupt.h>

#define PIN_COUNTER_1 9
#define PIN_COUNTER_2 10
#define PIN_COUNTER_3 11

#define PIN_STATUS_1 7
#define PIN_STATUS_2 8

#define PIN_ANALOG_1 1
#define PIN_ANALOG_2 2
#define PIN_ANALOG_3 3
#define PIN_ANALOG_4 4
#define PIN_ANALOG_5 5

#define DELAY_PRINT 1000
#define DEBOUNCE 10

volatile unsigned long c1 = 0;
uint32_t c1_time = 0;
volatile unsigned long c2 = 0;
uint32_t c2_time = 0;
volatile unsigned long c3 = 0;
uint32_t c3_time = 0;

volatile unsigned long s1 = 0;
volatile unsigned long s2 = 0;

float a1 = 0;
float a2 = 0;
float a3 = 0;
float a4 = 0;
float a5 = 0;

int t = 0;
int last_t = 0;

void output() {
  if(t - last_t < DELAY_PRINT) {
    return;
  }
  
  Serial.print("C1\t");
  Serial.println(c1);
  Serial.print("C2\t");
  Serial.println(c2);
  Serial.print("C3\t");
  Serial.println(c3);
  
  Serial.print("S1\t");
  Serial.println(s1);
  Serial.print("S2\t");
  Serial.println(s2);
  
  Serial.print("A1\t");
  Serial.println(a1);
  Serial.print("A2\t");
  Serial.println(a2);
  Serial.print("A3\t");
  Serial.println(a3);
  Serial.print("A4\t");
  Serial.println(a4);
  Serial.print("A5\t");
  Serial.println(a5);
  
  Serial.print("\n\n");
  
  last_t = t;
}

void getA() {
  a1 = analogRead(PIN_ANALOG_1);
  a2 = analogRead(PIN_ANALOG_2);
  a3 = analogRead(PIN_ANALOG_3);
  a4 = analogRead(PIN_ANALOG_4);
  a5 = analogRead(PIN_ANALOG_5);
}

void interrupt_handler_c1() {
  uint32_t i_time = millis();
  if(i_time - c1_time > DEBOUNCE) {
    c1++;
  }
  c1_time = i_time;
}
void interrupt_handler_c2() {
  uint32_t i_time = millis();
  if(i_time - c2_time > DEBOUNCE) {
    c2++;
  }
  c2_time = i_time;
}
void interrupt_handler_c3() {
  uint32_t i_time = millis();
  if(i_time - c3_time > DEBOUNCE) {
    c3++;
  }
  c3_time = i_time;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.setTimeout(1000);

  String cal = "";
  while(cal == "") {
    Serial.println("CAL");
    cal = Serial.readString();
    cal.trim();

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  sscanf(cal.c_str(), "C1:%lu C2:%lu C3:%lu", &c1, &c2, &c3);

  delay(500);
  Serial.println("CALOK");

  pinMode(PIN_COUNTER_1, INPUT_PULLUP);
  pinMode(PIN_COUNTER_2, INPUT_PULLUP);
  pinMode(PIN_COUNTER_3, INPUT_PULLUP);
  
  pinMode(PIN_STATUS_1, INPUT_PULLUP);
  pinMode(PIN_STATUS_2, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);  
  
  enableInterrupt(PIN_COUNTER_1, interrupt_handler_c1, FALLING);
  enableInterrupt(PIN_COUNTER_2, interrupt_handler_c2, FALLING);
  enableInterrupt(PIN_COUNTER_3, interrupt_handler_c3, FALLING);
}

void loop() {
  t = millis();
  getA();
  output();
  delay(100);
}
