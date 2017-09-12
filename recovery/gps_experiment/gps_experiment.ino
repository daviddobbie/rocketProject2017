#include <TinyGPS++.h>
#include <i2c_t3.h>
 #include <SoftwareSerial.h>


#define RXPIN 7
#define TXPIN 8

#define gpsSerialBaud 9600
#define gpsDecimalPoints 6

SoftwareSerial gpsSerial(RXPIN,TXPIN); // RX, TX (TX not used)

TinyGPSPlus gps;



void setupGPS() {
  for (int i = 0; !gps.location.isUpdated(); i++) {
    readGPS();
    delay(20);
    // Only print locking information every 50 iterations
    //Serial.println(gps.charsProcessed());    
    if (i%50 == 0) {
      Serial.println("Checksum passed/failed: " + String(gps.passedChecksum(), DEC) + "/" + String(gps.failedChecksum(), DEC) + " Sats in view: " + gps.satellites.value());
      Serial.println(getGPSLogString());
      Serial.println(getGPSTimeString());      
      Serial.println("Sentences with fix: " + (String)gps.sentencesWithFix());
}
  }
}

void readGPS() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
      
  }
}
// Function for returning formatted GPS string
String getGPSLogString() {
  return (String(gps.location.lat(), gpsDecimalPoints) + "," + String(gps.location.lng(), gpsDecimalPoints));
}

String getGPSTimeString() {
  return (String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));

}


void setup() {
  gpsSerial.begin(gpsSerialBaud);
  Serial.begin(9600);
  setupGPS();

}

void loop() {
    readGPS();
    //Serial.println(getGPSLogString());
    delay(100);
}

