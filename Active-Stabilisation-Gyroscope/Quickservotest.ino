// Sweep
// by BARRAGAN <http://barraganstudio.com> 

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
 
void setup() 
{ 
  servoX.attach(23);  // attaches servoX on pin 23 
  servoY.attach(14);  // attaches servoY on pin 14
} 
 
void loop() 
{ 
int Xcenter = 95;
int Ycenter = 100;
  
int Xmax = 120;//120
int Ymax = 125;//130
int Xmin = 70;//60
int Ymin = 75;//60
  
  servoX.write(Xmax);
  servoY.write(Ymax); // Move both X and Y servos to 10 degrees
  delay(1000);      // wait 1 sec

  servoX.write(Xmax);
  servoY.write(Ymin); // Move both X and Y servos to 10 degrees
  delay(1000);      // wait 1 sec
  
  servoX.write(Xmin);
  servoY.write(Ymin); //move both X and Y servos to 170 degrees
  delay(1000);       //wait 1 sec

  servoX.write(Xmin);
  servoY.write(Ymax); // Move both X and Y servos to 10 degrees
  delay(1000);      // wait 1 sec
  /*for(pos = 10; pos < 170; pos += 1)  // goes from 10 degrees to 170 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  */
} 

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

