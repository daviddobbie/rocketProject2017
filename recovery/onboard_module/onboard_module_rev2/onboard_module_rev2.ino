boolean isRev3 = false; //revision 3 of pcb
boolean isRev2 = true; //revision 2 of pcb

#include <RH_RF95.h> // this uses the radiohead arduino library http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
//#include <i2c_t3.h>
#include <TinyGPS++.h>


//GPS SETUP CODE
//in rev3 PCB greenwired: 24 GPS_ON_OFF; I2C_DIO and I2C_CLK pulled high with 2.2K VOUT
// note: pins lead to Serial4 being used for GPS communication
#define RXPIN 31
#define TXPIN 32
#define GPS_ON_OFF 24
#define gpsSerial Serial4
#define gpsSerialBaud 4800

#define TOP_ANT 33
#define BOT_ANT 34

// defines the IMU pins
#define IMU_SDA 38
#define IMU_SCL 37

#define usbSerialBaud 9600
#define gpsDecimalPoints 6

char serialOut = 0;
String LNG = 0;
String LAT = 0;
String TIME = 0;

boolean debug = false;
TinyGPSPlus gps;

 //RADIO SETUP CODE
// Specfies the frequency transmitted from the RFM96W radio
#define RF95_FREQ 434.0

/*REVISION 3 PCB
  #define RFM95_CS 10
  #define RFM95_RST 9
  #define RFM95_INT 28
  */
/* REVISION 2 PCB*/
  #define RFM95_CS 10
  #define RFM95_RST 0
  #define RFM95_INT 1


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);


// sets the sd card to be the one built into the Teensy 3.6
const int dataLogChipSelect = BUILTIN_SDCARD;
int piezo = 18;
File dataFile;

/**
 * sets up the radio, gps, and the sd card
 */
void setup() {
  if(isRev3){
    SPI.setSCK(27);
    SPI.begin(); 
  }
  
    /* sets up the radio for the controller*/
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
  
    pinMode(piezo, OUTPUT);

    Serial.begin(usbSerialBaud);
    Serial.println("CSV FORMAT: LONGITUDE, LATITUDE, GPS TIME, GPS CHARS PROCESSED");
    radioSetup();
    setupGPS();
    setupCard();

    
}

/**
 * Radio and gps being called and used
 */
void loop() {
  buzzer();
  //digitalWrite(led,HIGH); // tests that the controller board is operating
  readGPS();
  if(debug){
      Serial.println("Location: " + getGPSLogString() + " Time:" + getGPSTimeString());
  }
  LNG = String(gps.location.lng(), gpsDecimalPoints);
  LAT = String(gps.location.lat(), gpsDecimalPoints);
  TIME = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
  String gpsCoords =LNG + ", " + LAT + ", " + TIME + ", " + gps.charsProcessed();
  String msg = gpsCoords; // in csv file format for simple export to other formats
  Serial.println(msg);
  writeToSD(msg);
  transmitRadio(msg);
  if(debug){
          Serial.println("Checksum passed/failed: " + String(gps.passedChecksum(), DEC) + "/" + String(gps.failedChecksum(), DEC) + " Sats in view: " + gps.satellites.value());
          Serial.println(getGPSLogString());
          Serial.println(getGPSTimeString());    
          Serial.println("CharsProcessed: " + (String)gps.charsProcessed());  
          Serial.println("Sentences with fix: " + (String)gps.sentencesWithFix());    
  }

}

/**
 *  Make an annoying tone every second or so.
 */
void buzzer() {
    tone(piezo, 1275);

}
void activateTopAntenna(){
    digitalWrite(BOT_ANT, LOW);
    digitalWrite(TOP_ANT, HIGH);
}

void activateBotAntenna(){
    digitalWrite(TOP_ANT, LOW);
    digitalWrite(BOT_ANT, HIGH);
}

void setupGPS() {
  // activates the top antenna on the system
    pinMode(TOP_ANT, OUTPUT);
    pinMode(BOT_ANT, OUTPUT);
    activateTopAntenna(); //turns on antenna

   //sets up the activate pulse for the system
    pinMode(GPS_ON_OFF, OUTPUT);
    
    digitalWrite(GPS_ON_OFF, HIGH); //sets enable on off pulse to start GPS operation
    digitalWrite(TXPIN, HIGH);  
    delay(200);
    digitalWrite(GPS_ON_OFF, LOW);
    digitalWrite(TXPIN, LOW);
    delay(200);
    digitalWrite(GPS_ON_OFF, HIGH); //sets enable on off pulse to start GPS operation
    digitalWrite(TXPIN, HIGH);  
    delay(200);
    digitalWrite(GPS_ON_OFF, LOW);
    digitalWrite(TXPIN, LOW);
  //begins serial communication with the on board gps
    gpsSerial.begin(gpsSerialBaud,SERIAL_8N1);

}

void readGPS() {
  while (gpsSerial.available()) {
   serialOut = gpsSerial.read();
   //Serial.println((unsigned char)serialOut, HEX); 
   gps.encode(serialOut); //loads the stream of data to the tinyGPS library
 
  }
}
// Function for returning formatted GPS string
String getGPSLogString() {
  return (String(gps.location.lat(), gpsDecimalPoints) + "," + String(gps.location.lng(), gpsDecimalPoints));
}

String getGPSTimeString() {
  return (String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));

}
/*
 * transmits the radio signal from the Teensy. This will be picked up by the SDR on the 
 * laptop or to another Teensy radio.
 */
void transmitRadio(String message){
  int msgLength = message.length();
  char msgChar[msgLength];
  message.toCharArray(msgChar, msgLength+1); // converts the message into the appropriate char array for transmission
  if(debug) 
  Serial.println("Message: "+ message);
  if(debug) Serial.println("Transmitting..."); // Send a message to rf95_server
  
  if(debug) Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)msgChar, 40);
 
  if(debug) Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  if(debug) Serial.println("Sent");
}
/*
 * receives the radio signal from a teensy. This allows for data communication 
 * from the radio to transmit GPS data and the like
 */
void receiveRadio(){
   if (rf95.available()) // only checks if the radio is available for us to receive
   {
     // Should be a message for us now   
     uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
     uint8_t len = sizeof(buf);
    
     if (rf95.recv(buf, &len))
     {
       RH_RF95::printBuffer("Received: ", buf, len);
       Serial.print("Got: ");
       Serial.println((char*)buf);
       Serial.print("RSSI: ");
       Serial.println(rf95.lastRssi(), DEC); 
      }
   }
}

/*
 * This sets up the radio for the system. It sets the frequency it operates on to be 434.0 MHz
 * adapted from https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/rfm9x-test
 */
void radioSetup(){
    // manually resets the radio pin
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    while (!rf95.init()) {
       Serial.println("LoRa radio init failed");
       while (1);
    }
    Serial.println("LoRa radio init OK!");

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(RF95_FREQ)) {
      Serial.println("setFrequency failed");
      while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    rf95.setTxPower(23, false);
}
/*
 * Sets up the SD card to record data on it
 */
void setupCard(){
   Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(dataLogChipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized."); 
  return;
}

/**
 * this sends a string from where it is called and records it on the SD card
 */
void writeToSD(String str){
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(str);
    dataFile.close();
    // print to the serial port too:
    if(debug) Serial.println("Writing to storage...");
  }  
  // if the file isn't open, pop up an error:
  else {
    if(debug)Serial.println("Error writing to storage");
  } 
}

/**
 * gets the time string set up to be logged
 */
String currentTime(){
   String t;
   t += hour();
   t += printDigits(minute());
   t += printDigits(second());
   return t;
}
/* makes the time on the Teensy printable*/
String printDigits(int digits){ // taken from example teensy time
  String str = ":";
  if(digits < 10)
    str += "0";
  str += digits;
  return str;
}


