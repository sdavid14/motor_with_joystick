/*.................................
BTS7960 Motor Driver with Joystick
Written By : Steve David
*/

// motor control pins

// PWM pins control speed and direction.
int leftRPWM=9;   //  positive value on RPWM pin (and 0 on LPWM pin) means backward, 1=slowest, 255=fastest
int leftLPWM=10;  //  positive value on LPWM pin (and 0 on RPWM pin) means forward, 1=slowest 255=fastest
int rightRPWM=5;  // same as above, but right motor
int rightLPWM=6;  

//int leftL_EN=7;  no need to enable EN pins - we'll just connect R and L EN pins to breadboard 5v to enable, to conserve PWM pins on-board to allow room for multiple controllers.
//int leftR_EN=8;
//int rightL_EN=3;
//int rightR_EN=4;


int sensorPinX = A3; // Input pin for joystick, x value
int sensorPinY = A4; // Input pin for joystick, y value

void setup() {
  
  // output pins
  for(int i=1;i<10;i++){
   pinMode(i,OUTPUT);
  }
   for(int i=1;i<10;i++){
   digitalWrite(i,LOW);
  }

  delay(1000);
  Serial.begin(9600);
}

void loop() {
  int sensorValueX= analogRead(sensorPinX);
  int yValue = -1*(512-sensorValueX-2); // -2 is calibration for joystick.  switch x/y for orientation of joystick. switch +/-
  
  int sensorValueY= analogRead(sensorPinY);
  int xValue = -1*(512-sensorValueY+17); // +17 is calibration for joystick.  switch x/y for orientation of joystick. switch +/-

  int angle=0;
  int leftMotorPower; // this is the directional value for left motor power, -255 (backwards) to 255 (forward)
  int rightMotorPower; // same, but right motor
  int maxPower=255;


  delay(50);

if (abs(xValue)<5 && abs(yValue)<5) {
  Serial.print("STOPPED.  ");
} else {
  Serial.print("MOVING.   ");

  // Calculate angle the joystick is pushed.
  angle=atan2(abs(xValue), abs(yValue)) * 57.2958;

  // How far is the joystick pushed in 'angle' direction
  float speedRatio = (float(abs(yValue) + abs(xValue))/512);
  if (speedRatio>1) {
    speedRatio=1;
  }

  if (xValue>=0) {
    if (yValue>=0) {
      // forward right.  angle 0=both motors full forward.  45=left full forward, right stopped.  90=left motor full forward, right motor full backward.
      leftMotorPower = maxPower * speedRatio;
      rightMotorPower = (float(abs(45-angle))/45) * maxPower * speedRatio;
      if (angle>45) {
        rightMotorPower*=-1;
      }
      
    } else {
      // backward right.
     leftMotorPower = (float(abs(45-angle))/45) * maxPower * speedRatio;  // at 90 degrees, we are full forward. at 45 degrees we are stopped.  at 0 degrees, we are full backward.
     if (angle<45) {
      leftMotorPower*=-1;
     }

     rightMotorPower = -maxPower * speedRatio;  /// leave as is.

    }
  } else {
    if (yValue>=0) {
      // forward left
     // angle 0=both motors full forward.  45=right full forward, left stopped.  90=right motor full forward, left motor full backward.
      rightMotorPower = maxPower * speedRatio;
      leftMotorPower = (float(abs(45-angle))/45) * maxPower * speedRatio;
      if (angle>45) {
        leftMotorPower*=-1;
      }

    } else {
      // backward left
     leftMotorPower = -maxPower * speedRatio; // leave as-is
     
     rightMotorPower = (float(abs(45-angle))/45) * maxPower * speedRatio;  // at 90 degrees, we are full forward. at 45 degrees we are stopped.  at 0 degrees, we are full backward.
     if (angle<45) {
      rightMotorPower*=-1;
     }

    }
    
  }

}


  // now, write correct values to motor PWM pins.  
  // right motor
  if (rightMotorPower==0) {
    analogWrite(rightLPWM, 0); 
    analogWrite(rightRPWM, 0); 
  } else if (rightMotorPower>0) {
    analogWrite(rightLPWM, rightMotorPower); 
    analogWrite(rightRPWM, 0); 
  } else {
    analogWrite(rightLPWM, 0); 
    analogWrite(rightRPWM, -rightMotorPower); 
  }
  
  // left motor
  if (leftMotorPower==0) {
    analogWrite(leftLPWM, 0); 
    analogWrite(leftRPWM, 0); 
  } else if (leftMotorPower>0) {
    analogWrite(leftLPWM, leftMotorPower); 
    analogWrite(leftRPWM, 0); 
  } else {
    analogWrite(leftLPWM, 0); 
    analogWrite(leftRPWM, -leftMotorPower); 
  }

  // Write out to console.  Useful for debugging.
  Serial.print("X:");
  Serial.print(xValue);
  Serial.print(",Y:");
  Serial.print(yValue);
  Serial.print(", leftMotorPower:");
  Serial.print(leftMotorPower);
  Serial.print(", rightMotorPower:");
  Serial.print(rightMotorPower);
  Serial.print(", Angle: ");
  Serial.println(angle);

  
}


