// Extremely simple blink script to test code is compiled and sent correctly.

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(900);
	digitalWrite(LED_BUILTIN, LOW);
	delay(100);
}