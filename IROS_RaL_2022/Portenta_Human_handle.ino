//////============for Wifi communication

//#include<WiFi.h>
//#include<string.h>
//
//// Setup WiFi Characteristics
//#ifndef STASSID
//#define STASSID "Quadcopter" // Name of your router
//#define STAPSK  "psshcrlab" // Password of your router
//#endif
//
//const char* ssid      = STASSID;
//const char* password  = STAPSK;
//const int   port      = 49770;
//int status            = WL_IDLE_STATUS;
//IPAddress server(192,168,137,15);
//
//// Class Instances
//WiFiClient client;

//////============for Wifi communication

////============for IMU data
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO080 myIMU;
//float roll = 0.0;
//float pitch= 0.0;
//float yaw  = 0.0;
char buffer[41];
float roll, pitch, yaw;

////============ for encoder data
//#include <SoftwareSerial.h>

String data_recv[3];
float encoder1 = 0.0;
float encoder2 = 0.0;
float on_off   = 0.0;
char sz[18];

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
Wire.setClock(400000); //Increase I2C data rate to 400kHz
//Wire.setClock(100000); //Increase I2C data rate to 400kHz
myIMU.enableGyroIntegratedRotationVector(1); //Send data update every 50ms
Serial.println(F("Gyro integrated rotation vector enabled"));
delay(1000);
////============ for IMU communication  
 
//////============for Wifi communication

//  tryAgain:
//  status = WiFi.begin(ssid,password);
//  
//  if ( status != WL_CONNECTED) {
//    Serial.println("Couldn't get a wifi connection");
//
//      digitalWrite(LEDR,LOW);
//      digitalWrite(LEDG,LOW);
//      digitalWrite(LEDB,HIGH);
//      goto tryAgain;
//  }
//  else {
//    Serial.println("Connected to wifi");
//
//    digitalWrite(LEDR,HIGH);                                        // Blink Portenta Red Light
//    digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
//    digitalWrite(LEDB,LOW);                                         // Blink Portenta Blue Light
//  
//    Serial.println("\nStarting connection...");
//    
//    if (client.connect(server, port)) {
//      Serial.println("connected");
//
//    }
//  }
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
  
//////============for Wifi communication

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
//      Serial.println("Hey Captain");
    }
    
//    if (Serial1.available()>0)
//    {
//        String handle_data = Serial1.readStringUntil('_');
//        char buf[sizeof(sz)];
//        handle_data.toCharArray(buf, sizeof(buf));
//        char *p = buf;
//        char *str;
//        int i= 0;
//        while ((str = strtok_r(p, ",", &p)) != NULL){
//          data_recv[i] = str;
//          i++;
//        }
//
//        on_off   = data_recv[0].toFloat();
//        encoder1 = data_recv[1].toFloat();
//        encoder2 = data_recv[2].toFloat();
//     }


//      Serial1.print(on_off,0);
//      Serial1.print(",");
//      Serial1.print(encoder1,0);
//      Serial1.print(",");
//      Serial1.print(encoder2,0);
//      Serial1.print(",");
      Serial1.print("/");
      Serial1.print(roll*180.0/PI*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(pitch*180.0/PI*100 + 500000,0);
      Serial1.print(",");
      Serial1.print(yaw*180.0/PI*100 + 500000,0);
      Serial1.println("_");
//    }

//    if (Serial1.available()>0)
//    {
//        
//        sprintf(buffer, "/%1.0f,%6.0f,%6.0f,%7.2f,%7.2f,%7.2f_",on_off ,encoder1 ,encoder2 ,roll* 180.0 / PI, pitch* 180.0 / PI, yaw* 180.0 / PI);
//////        Serial.println(buffer);     
//        Serial1.write(buffer);
////     }


//      Serial.print(on_off,0);
//      Serial.print(",");
//      Serial.print(encoder1,0);
//      Serial.print(",");
//      Serial.print(encoder2,0);
//      Serial.print(",");
      Serial.print("/");
      Serial.print(roll*180.0/PI*100 + 500000,0);
      Serial.print(",");
      Serial.print(pitch*180.0/PI*100 + 500000,0);
      Serial.print(",");
      Serial.print(yaw*180.0/PI*100 + 500000,0);
      Serial.println("_");

      digitalWrite(LEDR,HIGH);                                        // Blink Portenta Red Light
      digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
      digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light

      delayMicroseconds(1); //Tcs waiting for another read in

}
