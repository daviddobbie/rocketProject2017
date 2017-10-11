/**
 * Things to calibrate before launch
 * Make sure Xcentre and Ycentre are actually the centre of the servo position
 * If you have to switch the servo pins (servoX was origninally pin 14, servoY was pin 23)
 * make sure you also switch the servoXratio and servoYratio values
 *
 * Includes. Because we communicate to the MPU9250 over I2C, we need to include
 * both Wire.h & I2Cdev.h for this to work.
 */
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"
#include "quaternionFilters.h"
#include <Servo.h>

/**
 * Global & environmental variables are declared below for the express purpose
 * of allowing the devleper to tinker.
 */
int BAUD_RATE = 9600;
int SERIAL_OUTPUT_SPEED_IN_MS = 10;
#define ENVIRONMENT_IS_DEV false
#define AHRS true
#define SerialDebug false

/**
 * Define the chip used.
 */
 MPU9250 accelgyro;

int intPin = 12;
int myLed = 13;

//LED is off when teensy begins programming, turns on after desired time to
//indicate the IMU has recalibrated
int launchWait = 60000; // time between teensy reset and IMU recalibration in ms
int launchLED = 15;


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
 * Arduino setup, including beginning the serial output on 9600 baud, and initialize
 * an I2C connection.
 */
void setup() {
    Wire.begin();
    Serial.begin(BAUD_RATE);

    pinMode(launchLED, OUTPUT);

    digitalWrite(launchLED, LOW);

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
    }

    delay(launchWait);
    accelgyro.calibrateMPU9250(accelgyro.gyroBias, accelgyro.accelBias);

    //signal ready for launch
    digitalWrite(launchLED, HIGH);

}

/**
 *
 */
void loop() {
    // Sample data at 100Hz
    delay(SERIAL_OUTPUT_SPEED_IN_MS);

    // Selection of data from either IMU or mock IMU facade here.
    ImuSensorDataStruct IMUdata;
    IMUdata = fetchData();

    //if(SerialDebug)
    //{
      //SanitizedImuDataStruct outputData = transformValues(IMUdata);
      //outputToCereal(outputData);
    //}
    //else
    //{
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
  //  }
}


/**
 * Retrieves a subset of data from the MPU9250 IMU.
 *
 * @returns an IMU data struct containing actual pitch, roll, yaw, and acceleration
 * values, represented as int16_t's.
 */
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
