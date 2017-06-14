int piezo = 12;
int led = 13;
int readPin = 28;
/**
 *
 */
void setup() {
    pinMode(piezo, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(readPin, INPUT);
    Serial.begin(9600);
}

/**
 *
 */
void loop() {
  buzzer();
  digitalWrite(led,HIGH); // tests that the controller board is operating
 
  
  int value = analogRead(readPin);
  Serial.println(value);


}

/**
 *  Make an annoying tone every second or so.
 */
void buzzer() {
    tone(piezo, 1275);
    delay(1000);
    tone(piezo, 900);
    delay(1000);
}


void enableGPS(){
}

void disableGPS(){
  
}

float getLat(){
}

float getLon(){
}

float getElevation(){
}

float getAccuracy(){
}


struct GPSDataStruct{
  
};

struct GPSVelocityStruct{
  
};

