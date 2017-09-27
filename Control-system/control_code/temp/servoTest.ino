#include <Servo.h>

Servo servoX;// create servo object to control a servo on the X axis
Servo servoY;// y axis servo
                // a maximum of eight servo objects can be created

int Xcentre = 86;
int Ycentre = 135;

int Xmax = 120;
int Ymax = 135;
int Xmin = 70;
int Ymin = 65;

float servoOffsetRatio = 1.3;

void setup()
{
  servoX.attach(23);  // attaches servoX on pin 23
  servoY.attach(14);  // attaches servoY on pin 14
}

int i = 60;

void loop()
{

servoY.write(Ycentre);
delay(2000);
/*
  servoX.write(Xcentre);
  delay(2000);
  servoX.write(Xcentre + 20*servoOffsetRatio);
  delay(2000);

/*
  servoX.write(Xmax);
  servoY.write(Ycentre); // Move gyro to right
  delay(500);      // wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ycentre); // Move gyro to centre
  delay(500);      // wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ymax); //move gyro to top
  delay(500);       //wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ycentre); // Move gyro to centre
  delay(500);      // wait 500ms

  servoX.write(Xmin);
  servoY.write(Ycentre); // Move gyro to left
  delay(500);      // wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ycentre); // Move gyro to centre
  delay(500);      // wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ymin); //move gyro to bottomr
  delay(500);       //wait 500ms

  servoX.write(Xcentre);
  servoY.write(Ycentre); //move gyro to centre
  delay(500);       //wait 500ms

  servoX.write(Xmin);
  servoY.write(Ymax); // Move gyro to top right corner
  delay(1000);      // wait 1 sec

  servoX.write(Xmax);
  servoY.write(Ymax); // Move gyro to top left corner
  delay(1000);      // wait 1 sec

  servoX.write(Xmax);
  servoY.write(Ymin); // Move gyro to bottom left corner
  delay(1000);      // wait 1 sec

  servoX.write(Xmin);
  servoY.write(Ymin); //move gyro to bottom right corner
  delay(1000);       //wait 1 sec

  servoX.write(Xmin);
  servoY.write(Ymax); // Move gyro to top right corner
  delay(1000);      // wait 1 sec
  */
}
