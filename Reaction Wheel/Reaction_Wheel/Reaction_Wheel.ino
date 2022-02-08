#include <ESP32Servo.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu(Wire);
Servo wheel;

float setPoint = 0.0;
float error, angleZ, currentTime, elapsedTime, previousTime;
float lastError, rateError, cumError;
float output = 1488; //Neutral position

float Kp = 0.0 ;
float Kd = 0.0 ;
float Ki = 0.0 ;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
  wheel.attach(14);
}

void loop() {
  mpu.update();
  angleZ = mpu.getAngleZ();
  currentTime = millis();
  elapsedTime = (double)(currentTime - previousTime);

  error = setPoint - angleZ;
  cumError = cumError + error*elapsedTime;
  rateError = (error - lastError)/elapsedTime;

  output = 1488 + Kp*error + Ki*cumError + Kd*rateError;

  lastError = error;
  previousTime = currentTime;
  
  if(output < 1312) output = 1312;
  if (output > 1832) output = 1832;
  wheel.writeMicroseconds(output);
  Serial.println(output);
}
