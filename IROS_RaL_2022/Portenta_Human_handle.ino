////============for IMU data
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO080 myIMU;
BNO080 myIMU_Acc;
char buffer[41];
float roll, pitch, yaw, AccX, AccY, AccZ;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(2000000);

//// To stop the code until serial moniter open
//  while(!Serial){;}

////============ for IMU communication  
Wire.begin();
if (myIMU.begin() == false)
{
  Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
  // Blink Yellow 
  digitalWrite(LEDR,LOW);                                        // Blink Portenta Red Light
  digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
  digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
}

if (myIMU_Acc.begin() == false)
{
  Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
  // Blink Yellow 
  digitalWrite(LEDR,LOW);                                        // Blink Portenta Red Light
  digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
  digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
}

Wire.setClock(400000); //Increase I2C data rate to 400kHz
myIMU.enableGyroIntegratedRotationVector(1); //Send data update every 50ms
myIMU_Acc.enableLinearAccelerometer(1); //Send data update every 50ms

//Serial.println(F("Gyro integrated rotation vector enabled"));
delay(1000);

}

void loop()
{
    //// To access the data from the IMU
    if (myIMU.dataAvailable() == true)
    {
      float quatI = myIMU.getQuatI();
      float quatJ = myIMU.getQuatJ();
      float quatK = myIMU.getQuatK();
      float quatReal = myIMU.getQuatReal();

      yaw   = atan2(2.0f * (quatI * quatJ +quatReal * quatK), quatReal * quatReal + quatI * quatI - quatJ * quatJ - quatK * quatK);
      pitch = -asin(2.0f * (quatI * quatK - quatReal * quatJ));
      roll  = atan2(2.0f * (quatReal * quatI + quatJ * quatK), quatReal * quatReal - quatI * quatI - quatJ * quatJ + quatK * quatK);
    }

    if (myIMU_Acc.dataAvailable() == true)
    {
      AccX = myIMU_Acc.getLinAccelX();
      AccY = myIMU_Acc.getLinAccelY();
      AccZ = myIMU_Acc.getLinAccelZ();
    }
    
      Serial1.print("/");
      Serial1.print(roll*180.0/PI*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(pitch*180.0/PI*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(yaw*180.0/PI*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(AccX*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(AccY*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(AccZ*100 + 500000,0);
      Serial1.println("_");

      Serial.print("/");
      Serial.print(roll*180.0/PI*100 + 500000,0);
      Serial.print(",");
      Serial.print(pitch*180.0/PI*100 + 500000,0);
      Serial.print(",");
      Serial.print(yaw*180.0/PI*100 + 500000,0);
      Serial.print(",");
      Serial.print(AccX*100 + 500000,0);
      Serial.print(",");
      Serial.print(AccY*100 + 500000,0);
      Serial.print(",");
      Serial.print(AccZ*100 + 500000,0);
      Serial.println("_");

//      Serial.print(AccX);
//      Serial.print(",");
//      Serial.print(AccY);
//      Serial.print(",");
//      Serial.println(AccZ);

      digitalWrite(LEDR,HIGH);                                        // Blink Portenta Red Light
      digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
      digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light

      delayMicroseconds(1); //Tcs waiting for another read in

}
