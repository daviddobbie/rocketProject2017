#include <TinyGPS++.h>

//in rev3 PCB greenwired: 24 GPS_ON_OFF; I2C_DIO and I2C_CLK pulled high with 2.2K VOUT

// note: pins lead to Serial4 being used for GPS communication
#define RXPIN 31
#define TXPIN 32
#define GPS_ON_OFF 24
#define gpsSerial Serial4
#define gpsSerialBaud 4800

#define TOP_ANT 33
#define BOT_ANT 34

#define usbSerialBaud 4800
#define gpsDecimalPoints 6

char serialOut = 0;
String LNG = 0;
String LAT = 0;
String TIME = 0;

boolean debug = true;
TinyGPSPlus gps;

void activateTopAntenna(){
    digitalWrite(BOT_ANT, LOW);
    digitalWrite(TOP_ANT, HIGH);
}

void activateBotAntenna(){
    digitalWrite(TOP_ANT, LOW);
    digitalWrite(BOT_ANT, HIGH);
}
void setupGPS() {
    pinMode(TOP_ANT, OUTPUT);
    pinMode(BOT_ANT, OUTPUT);
    activateTopAntenna(); //turns on antenna
   
    pinMode(GPS_ON_OFF, OUTPUT);
    
    digitalWrite(GPS_ON_OFF, HIGH); //sets enable on off to start GPS operation
    digitalWrite(TXPIN, HIGH);  
    delay(200);
    digitalWrite(GPS_ON_OFF, LOW);
    digitalWrite(TXPIN, LOW);
  
    gpsSerial.begin(gpsSerialBaud,SERIAL_8N1);
    for (int i = 0; !gps.location.isUpdated(); i++) {
      delay(20);
      readGPS();
      if (i%50 == 0) {
        if(debug){
          Serial.println("Checksum passed/failed: " + String(gps.passedChecksum(), DEC) + "/" + String(gps.failedChecksum(), DEC) + " Sats in view: " + gps.satellites.value());
          Serial.println(getGPSLogString());
          Serial.println(getGPSTimeString());    
          Serial.println("CharsProcessed: " + (String)gps.charsProcessed());  
          Serial.println("Sentences with fix: " + (String)gps.sentencesWithFix());
        }
      }
    }
}

void readGPS() {
  while (gpsSerial.available()) {
   serialOut = gpsSerial.read();
   //Serial.println((unsigned char)serialOut, HEX); 
   gps.encode(serialOut);
 
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
  Serial.begin(usbSerialBaud);
  setupGPS();
}

void loop() {
    readGPS();
    if(debug){
      Serial.println("Location: " + getGPSLogString() + " Time:" + getGPSTimeString());
    }
    LNG = String(gps.location.lng(), gpsDecimalPoints);
    LAT = String(gps.location.lat(), gpsDecimalPoints);
    TIME = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
}

