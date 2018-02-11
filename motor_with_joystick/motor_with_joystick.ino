/*.................................
BTS7960 Motor Driver with Joystick
Written By : Steve David
*/

// Left motor control pins
int leftRPWM=5;
int leftLPWM=6;
int leftL_EN=7;
int leftR_EN=8;

// right motor control pins
int rightRPWM=1;
int rightLPWM=2;
int rightL_EN=3;
int rightR_EN=4;

int sensorPinX = A3; // Input pin for joystick, x value
int sensorPinY = A4; // Input pin for joystick, y value

void setup() {
  
  // output pins
  for(int i=5;i<9;i++){
   pinMode(i,OUTPUT);
  }
   for(int i=5;i<9;i++){
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

  // Turn on motor control.
  // Left
  digitalWrite(leftR_EN,HIGH);
  digitalWrite(leftL_EN,HIGH);

  // Right
  digitalWrite(rightR_EN,HIGH);
  digitalWrite(rightL_EN,HIGH);

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

      // FIXME now, write correct values to motor pins.  LPWM and RPWM depend on positive/negative direction.  Subroutine?
      analogWrite(leftLPWM, leftMotorPower); // left motor
      analogWrite(leftRPWM, 0);

      analogWrite(rightLPWM, rightMotorPower); // right motor
      analogWrite(rightRPWM, 0);

      
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


