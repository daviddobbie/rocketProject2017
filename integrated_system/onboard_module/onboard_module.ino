boolean isRev3 = true; //revision 3 of pcb
boolean isRev2 = false; //revision 2 of pcb

#include <RH_RF95.h> // this uses the radiohead arduino library http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
//#include <i2c_t3.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "MPU9250.h"
#include "quaternionFilters.h"
#include <Servo.h>

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

boolean debug = true;
TinyGPSPlus gps;

int SERIAL_OUTPUT_SPEED_IN_MS = 10;
#define ENVIRONMENT_IS_DEV false
#define AHRS true
#define SerialDebug false

 //RADIO SETUP CODE
// Specfies the frequency transmitted from the RFM96W radio
#define RF95_FREQ 434.0

/*REVISION 3 PCB*/
  #define RFM95_CS 10
  #define RFM95_RST 9
  #define RFM95_INT 28
  
/* REVISION 2 PCB
  #define RFM95_CS 10
  #define RFM95_RST 0
  #define RFM95_INT 1
*/

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);


// sets the sd card to be the one built into the Teensy 3.6
const int dataLogChipSelect = BUILTIN_SDCARD;
int piezo = 18;
File dataFile;

MPU9250 accelgyro;
boolean IMUOperational = false;


int intPin = 12;
int myLed = 13;

Servo servoX;// create servo object to control a servo on the X axis
Servo servoY;// y axis servo
int servoXpin = 14;
int servoYpin = 23;
// a maximum of eight servo objects can be created

int Xcentre = 90;
int Ycentre = 85;

//servo ratio values represent the ratio between the motor distance to the pivot point and the servo arm length.
//multiplying the desired angle by this value makes sure that the servo moves the right amount to move the
//MOTOR to the desired angle. The higher ratio value is for the servo further away from the motor pivot
//the lower ratio value is for the servo closer to the motor pivot
float servoXratio = 2.7;
float servoYratio = 1.7;

//servoOffsetRatio ratio is just due to the internal servo error. It was found that to move the servo 10 degrees you have to
//tell it to move ~13 degrees
float servoOffsetRatio = 1.3;

//variables for lead controller
float XinCurrent, XoutCurrent, XinPrevious, XoutPrevious = 0;
float YinCurrent, YoutCurrent, YinPrevious, YoutPrevious = 0;
 
 /**
  * Type definition of a struct that shall contain "raw" IMU sensor data with
  * little to no sanitization. Data types for the sixs vars are of uint16_t
  */
typedef struct ImuSensorDataStruct {
     float pitch;
     float roll;
     float yaw;
 } ImuSensorDataStruct;

 /**
  * A sanitized IMU sensor data struct that contains all values cast into Strings
  * for the express purpose of allowing a data output function to concatenate
  * Strings together to form an output that is acceptable for Serial.println.
  */
 typedef struct SanitizedImuDataStruct {
     String pitch;
     String roll;
     String yaw;
 } SanitizedImuDataStruct;


/**
 * sets up the radio, gps, and the sd card
 */
void setup() {
  if(isRev3){
    SPI.setSCK(27);
    SPI.begin(); 

    Wire.setSDA(IMU_SDA);
    Wire.setSCL(IMU_SCL);
  }
  
    /* sets up the radio for the controller*/
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
  
    pinMode(piezo, OUTPUT);

    Serial.begin(usbSerialBaud);
    Serial.println("CSV FORMAT: LONGITUDE, LATITUDE, TRANSMITTER'S TIME");
    radioSetup();
    setupGPS();
    setupCard();
    
    Wire.begin();
    
    //pinMode(launchLED, OUTPUT);
    //digitalWrite(launchLED, LOW);

    //Setup servo pins
    servoX.attach(servoXpin);  // attaches servoX on pin 23
    servoY.attach(servoYpin);  // attaches servoY on pin 14

    // Set up the interrupt pin for IMU, its set as active high, push-pull
    pinMode(intPin, INPUT);
    digitalWrite(intPin, LOW);
    pinMode(myLed, OUTPUT);
    digitalWrite(myLed, HIGH);

    

    accelgyro.calibrateMPU9250(accelgyro.gyroBias, accelgyro.accelBias);
    accelgyro.initMPU9250();

    if (true) {
        // Read the WHO_AM_I register, this is a good test of communication
        byte c = accelgyro.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
        Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
        Serial.print(" I should be "); Serial.println(0x71, HEX);

        if(c!=(byte)0x71){
          IMUOperational = true;
        }
    }    
        Serial.println("WORKING");


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

  String gpsCoords =LNG + ", " + LAT;
  String msg = gpsCoords; // in csv file format for simple export to other formats
  writeToSD(msg + ", " + currentTime());
  transmitRadio(msg);



      // Selection of data from either IMU or mock IMU facade here.
    if(IMUOperational){
      ImuSensorDataStruct IMUdata;
      IMUdata = fetchData();

      if(debug)
      {
        SanitizedImuDataStruct outputData = transformValues(IMUdata);
        outputToCereal(outputData);
      }
      else
      {
        //get current value of motor angle
        XinCurrent = IMUdata.pitch;
        YinCurrent = IMUdata.yaw;

        //apply lead controller
        XoutCurrent = 2.4*XinCurrent - 1.801*XinPrevious + 0.4004*XoutPrevious;
        YoutCurrent = 2.4*YinCurrent - 1.801*YinPrevious + 0.4004*YoutPrevious;
  
        outputCSV(XinCurrent, YinCurrent, XoutCurrent, YoutCurrent);
  
        //set motor gimbal limits
        if(XoutCurrent > 5)
          XoutCurrent = 5;
        if(XoutCurrent < -5)
          XoutCurrent = -5;
        if(YoutCurrent > 5)
          YoutCurrent = 5;
        if(YoutCurrent < -5)
          YoutCurrent = -5;
        //write values to servos
        //servoX ratio is ratio of motor angle to servo angle
        //Servo offset ratio is internal servo offset. Writing servoOffsetRatio degrees actually outputs 1 degree
        servoX.write(Xcentre - XoutCurrent*servoOffsetRatio*servoXratio);
        servoY.write(Ycentre - YoutCurrent*servoOffsetRatio*servoYratio);
  
        //Set previous values for next loop
        XoutPrevious = XoutCurrent;
        YoutPrevious = YoutCurrent;
        XinPrevious = XinCurrent;
        YinPrevious = YinCurrent;
      }
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

  Serial.println("Message: "+ message);
  if(debug) Serial.println("Transmitting..."); // Send a message to rf95_server
  
  if(debug) Serial.println("Sending...");
  rf95.send((uint8_t *)msgChar, 40);
 
  if(debug) Serial.println("Waiting for packet to complete...");;
  rf95.waitPacketSent(100);
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

ImuSensorDataStruct fetchData() {
    // If intPin goes high, all data registers have new data
      // On interrupt, check if data ready interrupt
      if (accelgyro.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
      {
        accelgyro.readAccelData(accelgyro.accelCount);  // Read the x/y/z adc values
        accelgyro.getAres();

        // Now we'll calculate the accleration value into actual g's
        // This depends on scale being set
        accelgyro.ax = (float)accelgyro.accelCount[0]*accelgyro.aRes; // - accelBias[0];
        accelgyro.ay = (float)accelgyro.accelCount[1]*accelgyro.aRes; // - accelBias[1];
        accelgyro.az = (float)accelgyro.accelCount[2]*accelgyro.aRes; // - accelBias[2];

        accelgyro.readGyroData(accelgyro.gyroCount);  // Read the x/y/z adc values
        accelgyro.getGres();

        // Calculate the gyro value into actual degrees per second
        // This depends on scale being set
        accelgyro.gx = (float)accelgyro.gyroCount[0]*accelgyro.gRes;
        accelgyro.gy = (float)accelgyro.gyroCount[1]*accelgyro.gRes;
        accelgyro.gz = (float)accelgyro.gyroCount[2]*accelgyro.gRes;

        accelgyro.readMagData(accelgyro.magCount);  // Read the x/y/z adc values
        accelgyro.getMres();
        // User environmental x-axis correction in milliGauss, should be
        // automatically calculated
        accelgyro.magbias[0] = +470.;
        // User environmental x-axis correction in milliGauss TODO axis??
        accelgyro.magbias[1] = +120.;
        // User environmental x-axis correction in milliGauss
        accelgyro.magbias[2] = +125.;

        // Calculate the magnetometer values in milliGauss
        // Include factory calibration per data sheet and user environmental
        // corrections
        // Get actual magnetometer value, this depends on scale being set
        accelgyro.mx = (float)accelgyro.magCount[0]*accelgyro.mRes*accelgyro.magCalibration[0] -
                   accelgyro.magbias[0];
        accelgyro.my = (float)accelgyro.magCount[1]*accelgyro.mRes*accelgyro.magCalibration[1] -
                   accelgyro.magbias[1];
        accelgyro.mz = (float)accelgyro.magCount[2]*accelgyro.mRes*accelgyro.magCalibration[2] -
                   accelgyro.magbias[2];
      } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

      // Must be called before updating quaternions!
      accelgyro.updateTime();

      // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
      // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
      // (+ up) of accelerometer and gyro! We have to make some allowance for this
      // orientationmismatch in feeding the output to the quaternion filter. For the
      // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
      // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
      // modified to allow any convenient orientation convention. This is ok by
      // aircraft orientation standards! Pass gyro rate as rad/s
    //  MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
      MadgwickQuaternionUpdate(accelgyro.ax, accelgyro.ay, accelgyro.az, accelgyro.gx*DEG_TO_RAD,
                             accelgyro.gy*DEG_TO_RAD, accelgyro.gz*DEG_TO_RAD, accelgyro.my,
                             accelgyro.mx, accelgyro.mz, accelgyro.deltat);

      if (!AHRS)
      {
        accelgyro.delt_t = millis() - accelgyro.count;
        if (accelgyro.delt_t > SERIAL_OUTPUT_SPEED_IN_MS)
        {
          if(SerialDebug)
          {
            // Print acceleration values in milligs!
            //Serial.print("X-acceleration: "); Serial.print(1000*accelgyro.ax);
            //Serial.print(" mg ");
            //Serial.print("Y-acceleration: "); Serial.print(1000*accelgyro.ay);
            //Serial.print(" mg ");
            //Serial.print("Z-acceleration: "); Serial.print(1000*accelgyro.az);
            //Serial.println(" mg ");

            // Print gyro values in degree/sec
            Serial.print("X-gyro rate: "); Serial.print(accelgyro.gx, 3);
            Serial.print(" degrees/sec ");
            Serial.print("Y-gyro rate: "); Serial.print(accelgyro.gy, 3);
            Serial.print(" degrees/sec ");
            Serial.print("Z-gyro rate: "); Serial.print(accelgyro.gz, 3);
            Serial.println(" degrees/sec");
          }

          accelgyro.count = millis();
        } // if (accelgyro.delt_t > 500)
      } // if (!AHRS)
      else
      {
        // Serial print and/or display at 0.5 s rate independent of data rates
        accelgyro.delt_t = millis() - accelgyro.count;

        if (accelgyro.delt_t > SERIAL_OUTPUT_SPEED_IN_MS)
        {
          if(SerialDebug)
          {
            Serial.print("ax = "); Serial.print((int)1000*accelgyro.ax);
            Serial.print(" ay = "); Serial.print((int)1000*accelgyro.ay);
            Serial.print(" az = "); Serial.print((int)1000*accelgyro.az);
            Serial.println(" mg");

            Serial.print("gx = "); Serial.print( accelgyro.gx, 2);
            Serial.print(" gy = "); Serial.print( accelgyro.gy, 2);
            Serial.print(" gz = "); Serial.print( accelgyro.gz, 2);
            Serial.println(" deg/s");

            Serial.print("q0 = "); Serial.print(*getQ());
            Serial.print(" qx = "); Serial.print(*(getQ() + 1));
            Serial.print(" qy = "); Serial.print(*(getQ() + 2));
            Serial.print(" qz = "); Serial.println(*(getQ() + 3));
          }

            // Define output variables from updated quaternion---these are Tait-Bryan
            // angles, commonly used in aircraft orientation. In this coordinate system,
            // the positive z-axis is down toward Earth. Yaw is the angle between Sensor
            // x-axis and Earth magnetic North (or true North if corrected for local
            // declination, looking down on the sensor positive yaw is counterclockwise.
            // Pitch is angle between sensor x-axis and Earth ground plane, toward the
            // Earth is positive, up toward the sky is negative. Roll is angle between
            // sensor y-axis and Earth ground plane, y-axis up is positive roll. These
            // arise from the definition of the homogeneous rotation matrix constructed
            // from quaternions. Tait-Bryan angles as well as Euler angles are
            // non-commutative; that is, the get the correct orientation the rotations
            // must be applied in the correct order which for this configuration is yaw,
            // pitch, and then roll.
            // For more see
            // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
            // which has additional links.
          accelgyro.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
                        *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
                        - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
          accelgyro.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
                        *(getQ()+2)));
          accelgyro.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
                        *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
                        - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
          accelgyro.pitch *= RAD_TO_DEG;
          accelgyro.yaw   *= RAD_TO_DEG;
          // Declination of Victoria University (41.2904° S 174.7687° E) is
          //  22.53E  ± 0.34° (or 22.5°) on 2017-06
          // -
          accelgyro.yaw   -= 22.5;
          accelgyro.roll  *= RAD_TO_DEG;

          if(SerialDebug)
          {
            Serial.print("Yaw, Pitch, Roll: ");
            Serial.print(accelgyro.yaw, 2);
            Serial.print(", ");
            Serial.print(accelgyro.pitch, 2);
            Serial.print(", ");
            Serial.println(accelgyro.roll, 2);

            Serial.print("rate = ");
            Serial.print((float)accelgyro.sumCount/accelgyro.sum, 2);
            Serial.println(" Hz");
          }

          accelgyro.count = millis();
          accelgyro.sumCount = 0;
          accelgyro.sum = 0;
        }
      }

    return {accelgyro.pitch, accelgyro.yaw, accelgyro.roll};
}

/**
 *
 */
SanitizedImuDataStruct transformValues(ImuSensorDataStruct data) {
    return { String(data.pitch), String(data.roll), String(data.yaw) };
}

/**
 * Outputs data over a Serial port as a JSON string, ready to be read by other
 * interested sources.
 *
 * @param outputData A sanitized struct containing data that should be output to
 * the Serial monitor.
 */
void outputToCereal(SanitizedImuDataStruct outputData) {
    String out = "{ \"pitch\": \"" + outputData.pitch + "\", \"roll\": \"" +
    outputData.roll + "\", \"yaw\": \"" + outputData.yaw + "\"}";

    Serial.println(out);

  }


/**
outputs CSV file to check that control system works
*/
void outputCSV(float Xin, float Yin, float Xout, float Yout)
{
  String out = "" + (String)Xin + "," + (String)Yin + "," + (String)Xout + "," + (String)Yout + "\n";
  Serial.println(out);

}
