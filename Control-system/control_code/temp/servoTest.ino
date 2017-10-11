#include <Servo.h>

Servo servoX;// create servo object to control a servo on the X axis
Servo servoY;// y axis servo
                // a maximum of eight servo objects can be created

int Xcentre = 90;
int Ycentre = 85;

int Xmax = 120;
int Ymax = 135;
int Xmin = 70;
int Ymin = 65;

float servoOffsetRatio = 1.3;

void setup()
{
  servoX.attach(14);  // attaches servoX on pin 23
  servoY.attach(23);  // attaches servoY on pin 14
}

int i = 60;

void loop()
{
servoX.write(Xcentre);
servoY.write(Ycentre);

delay(2000);

servoX.write(Xcentre + 10);
servoY.write(Ycentre + 10);

delay(2000);

}