

#include <Servo.h> 
 
Servo servoX;// create servo object to control a servo on the X axis 
Servo servoY;// y axis servo
                // a maximum of eight servo objects can be created 
 
//double Xkp = 0;
//double Xki = 0;

//double Ykp = 0;
//double Ykp = 0;

//double Xp = 0;
//double Xi = 0;
//double Xpi = 0;

//double Yp = 0;
//double Yi = 0;
//double Ypi = 0;

//double Xerror = 0;
//double Yerror = 0;

int Xcenter = 95;
int Ycenter = 100;
  
int Xmax = 120;
int Ymax = 125;
int Xmin = 70;
int Ymin = 75;
 
void setup() 
{ 
  servoX.attach(23);  // attaches servoX on pin 23 
  servoY.attach(14);  // attaches servoY on pin 14
} 
 
void loop() 
{ 
  
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
}

/*PID code for IMU implementation*/

/*working variables*/
//unsigned long lastTime;
//double Input, Output, Setpoint;
//double errSum, lastErr;
//double kp, ki, kd;
//void Compute()
//{
   /*How long since we last calculated*/
//   unsigned long now = millis();
//   double timeChange = (double)(now - lastTime);
  
   /*Compute all the working error variables*/
//   double error = Setpoint - Input;
//   errSum += (error * timeChange);
//   double dErr = (error - lastErr) / timeChange;
  
   /*Compute PID Output*/
//   Output = kp * error + ki * errSum + kd * dErr;
  
   /*Remember some variables for next time*/
//   lastErr = error;
//   lastTime = now;
//}
  
//void SetTunings(double Kp, double Ki, double Kd)
//{
//   kp = Kp;
//   ki = Ki;
//   kd = Kd;
//}

