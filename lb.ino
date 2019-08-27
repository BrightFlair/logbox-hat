#define EI_ARDUINO_INTERRUPTED_PIN
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
#define COOLOFF 10

volatile unsigned long c1 = 0;
volatile unsigned long last_c1 = 0;
volatile unsigned long c1_ftime = 0;
volatile unsigned long c1_rtime = 0;
volatile unsigned long c1_fstatus = 0;
volatile unsigned long c1_rstatus = 0;

volatile unsigned long c2 = 0;
volatile unsigned long last_c2 = 0;
volatile unsigned long c2_ftime = 0;
volatile unsigned long c2_rtime = 0;
volatile unsigned long c2_fstatus = 0;
volatile unsigned long c2_rstatus = 0;

volatile unsigned long c3 = 0;
volatile unsigned long last_c3 = 0;
volatile unsigned long c3_ftime = 0;
volatile unsigned long c3_rtime = 0;
volatile unsigned long c3_fstatus = 0;
volatile unsigned long c3_rstatus = 0;

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
	switch (num) {
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

	for (int i = 0; i < ANALOG_SMOOTHING; i++) {
		value += *(a + i);
	}

	return ((value / ANALOG_SMOOTHING));
}

void output() {
	if (t - last_t < DELAY_PRINT) {
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
	aIndex++;

	if (aIndex >= ANALOG_SMOOTHING) {
		aIndex = 0;
	}
}

void resetAnalog() {
	for (int i = 0; i < ANALOG_SMOOTHING; i++) {
		a1[i] = 0;
		a2[i] = 0;
		a3[i] = 0;
		a4[i] = 0;
		a5[i] = 0;
	}
}

// Only register a falling edge if it's long enough away from the previous rising edge (debounce)
void c1_fall() {
	unsigned long itime = millis();

	c1_fstatus = 1;

	// If a falling edge has been registered
	// and the time since last falling edge is less than debounce time,
	// ignore!
	if (c1_ftime > 0
	&& itime - c1_ftime < DEBOUNCE) {
		return;
	}

	c1_fstatus = 2;

	// If a rising edge has been registered
	// and the time since last rising edge is less than cooloff time,
	// ignore!
	if (c1_rtime > 0
	&& itime - c1_rtime < COOLOFF) {
		return;
	}

	c1_fstatus = 3;

	// Register a successful falling edge.
	c1_ftime = itime;
}

void c1_rise() {
	unsigned long itime = millis();

	c1_rstatus = 1;

	// If a rising edge has been registered
	// and the time since last rising edge is less than debounce time,
	// ignore!
	if (c1_rtime > 0
	&& itime - c1_rtime < DEBOUNCE) {
		return;
	}

	c1_rstatus = 2;

	// If the time since last falling edge is less than colloff time,
	// ignore!
	if (itime - c1_ftime < COOLOFF) {
		return; // Too soon - cool off!
	}

	c1_rstatus = 3;

	c1_rtime = itime;
	c1++;
}

void c1_change() {
	if (arduinoPinState == 0) {
		c1_fall();
	}
	else {
		c1_rise();
	}
}

// Only register a falling edge if it's long enough away from the previous rising edge (debounce)
void c2_fall() {
	unsigned long itime = millis();

	c2_fstatus = 1;

	// If a falling edge has been registered
	// and the time since last falling edge is less than debounce time,
	// ignore!
	if (c2_ftime > 0
	&& itime - c2_ftime < DEBOUNCE) {
		return;
	}

	c2_fstatus = 2;

	// If a rising edge has been registered
	// and the time since last rising edge is less than cooloff time,
	// ignore!
	if (c2_rtime > 0
	&& itime - c2_rtime < COOLOFF) {
		return;
	}

	c2_fstatus = 3;

	// Register a successful falling edge.
	c2_ftime = itime;
}

void c2_rise() {
	unsigned long itime = millis();

	c2_rstatus = 1;

	// If a rising edge has been registered
	// and the time since last rising edge is less than debounce time,
	// ignore!
	if (c2_rtime > 0
	&& itime - c2_rtime < DEBOUNCE) {
		return;
	}

	c2_rstatus = 2;

	// If the time since last falling edge is less than colloff time,
	// ignore!
	if (itime - c2_ftime < COOLOFF) {
		return; // Too soon - cool off!
	}

	c2_rstatus = 3;

	c2_rtime = itime;
	c2++;
}

void c2_change() {
	if (arduinoPinState == 0) {
		c2_fall();
	}
	else {
		c2_rise();
	}
}

// Only register a falling edge if it's long enough away from the previous rising edge (debounce)
void c3_fall() {
	unsigned long itime = millis();

	c3_fstatus = 1;

	// If a falling edge has been registered
	// and the time since last falling edge is less than debounce time,
	// ignore!
	if (c3_ftime > 0
	&& itime - c3_ftime < DEBOUNCE) {
		return;
	}

	c3_fstatus = 2;

	// If a rising edge has been registered
	// and the time since last rising edge is less than cooloff time,
	// ignore!
	if (c3_rtime > 0
	&& itime - c3_rtime < COOLOFF) {
		return;
	}

	c3_fstatus = 3;

	// Register a successful falling edge.
	c3_ftime = itime;
}

void c3_rise() {
	unsigned long itime = millis();

	c3_rstatus = 1;

	// If a rising edge has been registered
	// and the time since last rising edge is less than debounce time,
	// ignore!
	if (c3_rtime > 0
	&& itime - c3_rtime < DEBOUNCE) {
		return;
	}

	c3_rstatus = 2;

	// If the time since last falling edge is less than colloff time,
	// ignore!
	if (itime - c3_ftime < COOLOFF) {
		return; // Too soon - cool off!
	}

	c3_rstatus = 3;

	c3_rtime = itime;
	c3++;
}

void c3_change() {
	if (arduinoPinState == 0) {
		c3_fall();
	}
	else {
		c3_rise();
	}
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	while (!Serial);
	Serial.setTimeout(1000);

	String cal = "";
	while (cal == "") {
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

	enableInterrupt(PIN_COUNTER_1, c1_change, CHANGE);
	enableInterrupt(PIN_COUNTER_2, c2_change, CHANGE);
	enableInterrupt(PIN_COUNTER_3, c3_change, CHANGE);

	resetAnalog();
}

void loop() {
	t = millis();
	readAnalog();
	output();
	delay(10);
}
