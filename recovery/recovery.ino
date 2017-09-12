


#include <RH_RF95.h> // this uses the radiohead arduino library http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <i2c_t3.h>


#define RFM95_CS 10
#define RFM95_RST 0
#define RFM95_INT 1

#define GPS_SDA 38
#define GPS_SCL 37
 
// Specfies the frequency transmitted from the RFM96W radio
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
// sets the sd card to be the one built into the Teensy 3.6
const int dataLogChipSelect = BUILTIN_SDCARD;

int piezo = 18;
int led = 13;
int readPin = 5;

File dataFile;
TinyGPSPlus gps;
/**
 *
 */
void setup() {
    /* sets up the radio for the controller*/
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
  
    pinMode(piezo, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(readPin, INPUT);
    Serial.begin(9600);

    radioSetup();
    setupCard();

    
}

/**
 *
 */
void loop() {
  buzzer();
  //digitalWrite(led,HIGH); // tests that the controller board is operating

  
  String gpsCoords = "00_00_00_00";
  String msg = "g" + gpsCoords + "t" + currentTime(); 
  writeToSD(msg);
  transmitRadio(msg);


}

/**
 *  Make an annoying tone every second or so.
 */
void buzzer() {
    tone(piezo, 1275);

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

/*
 * transmits the radio signal from the Teensy. This will be picked up by the SDR on the 
 * laptop or to another Teensy radio.
 */
void transmitRadio(String message){
  int msgLength = message.length();
  char msgChar[msgLength];
  message.toCharArray(msgChar, msgLength+1); // converts the message into the appropriate char array for transmission

  Serial.println("Message: "+ message);
  delay(50); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)msgChar, 20);
 
  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  Serial.println("Sent");
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
struct GPSDataStruct{
  
};
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
    Serial.println("Writing to storage...");
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error writing to storage");
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

String printDigits(int digits){ // taken from example teensy time
  String str = ":";
  if(digits < 10)
    str += "0";
  str += digits;
  return str;
}
struct GPSVelocityStruct{
  
};

