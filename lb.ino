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

unsigned long c1 = 0;
unsigned long c2 = 0;
unsigned long c3 = 0;

unsigned long last_c1 = 0;
unsigned long last_c2 = 0;
unsigned long last_c3 = 0;

unsigned int s1 = 0;
unsigned int s2 = 0;

float a1 = 0;
float a2 = 0;
float a3 = 0;
float a4 = 0;
float a5 = 0;

int t = 0;
int last_t = 0;

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
	last_c1 = HIGH;
	last_c2 = HIGH;
	last_c3 = HIGH;

	delay(500);
	Serial.println("CALOK");

        pinMode(PIN_COUNTER_1, INPUT_PULLUP);
        pinMode(PIN_COUNTER_2, INPUT_PULLUP);
        pinMode(PIN_COUNTER_3, INPUT_PULLUP);

        pinMode(PIN_STATUS_1, INPUT_PULLUP);
        pinMode(PIN_STATUS_2, INPUT_PULLUP);
	digitalWrite(LED_BUILTIN, HIGH);
}

void getC() {
        unsigned int val_c1 = digitalRead(PIN_COUNTER_1);
        if(val_c1 == HIGH
        && val_c1 != last_c1) {
                c1 ++;
        }
        last_c1 = val_c1;

        unsigned int val_c2 = digitalRead(PIN_COUNTER_2);
        if(val_c2 == HIGH
        && val_c2 != last_c2) {
                c2 ++;
        }
        last_c2 = val_c2;

        unsigned int val_c3 = digitalRead(PIN_COUNTER_3);
        if(val_c3 == HIGH
        && val_c3 != last_c3) {
                c3 ++;
        }
        last_c3 = val_c3;
}

void getS() {
        s1 = digitalRead(PIN_STATUS_1);
        s2 = digitalRead(PIN_STATUS_2);
}

void getA() {
        a1 = analogRead(PIN_ANALOG_1);
        a2 = analogRead(PIN_ANALOG_2);
        a3 = analogRead(PIN_ANALOG_3);
        a4 = analogRead(PIN_ANALOG_4);
        a5 = analogRead(PIN_ANALOG_5);
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

void loop() {
        t = millis();
        getC();
        getS();
        getA();
        output();
        delay(5);
}
