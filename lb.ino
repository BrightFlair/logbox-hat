#include <pcint.h>

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

volatile unsigned long c1 = 0;
volatile unsigned long c2 = 0;
volatile unsigned long c3 = 0;

int s1 = 0;
int s2 = 0;

float a1 = 0;
float a2 = 0;
float a3 = 0;
float a4 = 0;
float a5 = 0;

void tick_c1() {
	c1 ++;
}

void tick_c2() {
	c2 ++;
}

void tick_c3() {
	c3 ++;
}

void setup() {
	Serial.begin(9600);
	while(!Serial);

	pinMode(PIN_COUNTER_1, INPUT_PULLUP);
	pinMode(PIN_COUNTER_2, INPUT_PULLUP);
	pinMode(PIN_COUNTER_3, INPUT_PULLUP);
	PCattachInterrupt<PIN_COUNTER_1>(tick_c1, RISING);
	PCattachInterrupt<PIN_COUNTER_2>(tick_c2, RISING);
	PCattachInterrupt<PIN_COUNTER_3>(tick_c3, RISING);

	pinMode(PIN_STATUS_1, INPUT_PULLUP);
	pinMode(PIN_STATUS_2, INPUT_PULLUP);
}

void loop() {
	Serial.print("C1\t");
	Serial.println(c1);
	Serial.print("C2\t");
	Serial.println(c2);
	Serial.print("C3\t");
	Serial.println(c3);

	s1 = digitalRead(PIN_STATUS_1);
	s2 = digitalRead(PIN_STATUS_2);
	Serial.print("S1\t");
	Serial.println(s1);
	Serial.print("S2\t");
	Serial.println(s2);
	delay(5000);

	a1 = analogRead(PIN_ANALOG_1);
	a2 = analogRead(PIN_ANALOG_2);
	a3 = analogRead(PIN_ANALOG_3);
	a4 = analogRead(PIN_ANALOG_4);
	a5 = analogRead(PIN_ANALOG_5);
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
	
	Serial.print("\n");
}
