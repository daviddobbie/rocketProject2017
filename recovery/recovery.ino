int piezo = 6;

/**
 *
 */
void setup() {
    pinMode(piezo, OUTPUT);
}

/**
 *
 */
void loop() {
    buzzer();
}

/**
 *  Make an annoying tone every second or so.
 */
void buzzer() {
    tone(piezo, 1275);
    delay(1000);
    tone(piezo, 1500);
    delay(1000);
}