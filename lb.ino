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

const unsigned int ANALOG_SMOOTHING = 100;
unsigned int *a;
unsigned int a1[ANALOG_SMOOTHING];
unsigned int a2[ANALOG_SMOOTHING];
unsigned int a3[ANALOG_SMOOTHING];
unsigned int a4[ANALOG_SMOOTHING];
unsigned int a5[ANALOG_SMOOTHING];
unsigned int aIndex = 0;

unsigned int t = 0;
unsigned int last_t = 0;

float getA(int num) {
	switch(num) {
	case 1:
		a = a1;
		break;
	case 2:
		a = a2;
		break;
	case 3:
		a = a3;
		break;
	case 4:
		a = a4;
		break;
	case 5:
		a = a5;
		break;
	}

	float value = 0;

	for(int i = 0; i < ANALOG_SMOOTHING; i++) {
		value += *(a + i);
	}

	return ((value / ANALOG_SMOOTHING));
}

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
	Serial.println(getA(1));
	Serial.print("A2\t");
	Serial.println(getA(2));
	Serial.print("A3\t");
	Serial.println(getA(3));
	Serial.print("A4\t");
	Serial.println(getA(4));
	Serial.print("A5\t");
	Serial.println(getA(5));

	Serial.print("\n\n");

	last_t = t;
}

void readAnalog() {
	a1[aIndex] = analogRead(PIN_ANALOG_1);
	a2[aIndex] = analogRead(PIN_ANALOG_2);
	a3[aIndex] = analogRead(PIN_ANALOG_3);
	a4[aIndex] = analogRead(PIN_ANALOG_4);
	a5[aIndex] = analogRead(PIN_ANALOG_5);
	aIndex ++;

	if(aIndex >= ANALOG_SMOOTHING) {
		aIndex = 0;
	}
}

void resetAnalog() {
	for(int i = 0; i < ANALOG_SMOOTHING; i++) {
		a1[i] = 0;
		a2[i] = 0;
		a3[i] = 0;
		a4[i] = 0;
		a5[i] = 0;
	}
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

	resetAnalog();
}

void loop() {
	t = millis();
	readAnalog();
	output();
	delay(10);
}