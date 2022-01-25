#include<WiFi.h>
#include<string.h>
#include <Wire.h>
String data_recv[2];
float enc1,enc2 = 0.0;
char sz[16];
char dataTransmit[39] ;

#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080

// Setup WiFi Characteristics_________________________________________________________________________________________________
#ifndef STASSID
#define STASSID "Quadcopter" // Name of your router
#define STAPSK  "hcrlab1234" // Password of your router
#endif
const char* ssid      = STASSID;
const char* password  = STAPSK;
const int   port      = 49200;
int status            = WL_IDLE_STATUS;
IPAddress server(192,168,3,1);
WiFiClient client;

//============================================================================================================================
// Class Instances____________________________________________________________________________________________________________
BNO080 myIMU;
//============================================================================================================================
// Variable Initialization____________________________________________________________________________________________________
int counter = 0;
float roll, pitch, yaw;
float startTime = 0.0;
//____________________________________________________________________________________________________________________________


void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println();
  Serial.println("BNO080 Read Example");
  while(!Serial){
    ;
  }
  Wire.begin();
  if (myIMU.begin() == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    // Blink Yellow ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    digitalWrite(LEDR,LOW);                                        // Blink Portenta Red Light
    digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
    digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
    while (1);
  }
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  myIMU.enableGyroIntegratedRotationVector(1); //Send data update every 50ms
  Serial.println(F("Gyro integrated rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, gyroX, gyroY, gyroZ"));
  startTime = millis();


//Wifi and TCP Client Setup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  status = WiFi.begin(ssid,password);

delay(10000);
  
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while(true)
    {
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,HIGH);
    }
  }
  else {
    Serial.println("Connected to wifi");
    Serial.println("\nStarting connection...");
    if (client.connect(server, port)) {
      Serial.println("connected");
      pinMode(LEDR,HIGH);
      pinMode(LEDG,LOW);
      pinMode(LEDB,HIGH);
    }
  }
  printWiFiStatus();








}

void loop()
{

    readEncoder();


    if (myIMU.dataAvailable() == true)
    {
      float quatI = myIMU.getQuatI();
      float quatJ = myIMU.getQuatJ();
      float quatK = myIMU.getQuatK();
      float quatReal = myIMU.getQuatReal();

      float yaw   = atan2(2.0f * (quatI * quatJ +quatReal * quatK), quatReal * quatReal + quatI * quatI - quatJ * quatJ - quatK * quatK);
      float pitch = -asin(2.0f * (quatI * quatK - quatReal * quatJ));
      float roll = atan2(2.0f * (quatReal * quatI + quatJ * quatK), quatReal * quatReal - quatI * quatI - quatJ * quatJ + quatK * quatK);

      float data_1 = data_recv[0].toFloat();
      float data_2 = data_recv[1].toFloat();

      sprintf(dataTransmit,"%7.2f,%7.2f,%7.2f,%7.2f,%7.2f",data_1,data_2,roll* 180.0 / PI,pitch* 180.0 / PI,yaw* 180.0 / PI);
      client.write(dataTransmit);
      Serial.println(dataTransmit);

//    Serial.print(data_recv[0]); // 7
//    Serial.print(",");          // 1
//    Serial.print(data_recv[1]); // 7
//    Serial.print(",");          // 1
//    Serial.print(roll* 180.0 / PI, 2); // 7
//    Serial.print(",");                 // 1 
//    Serial.print(pitch* 180.0 / PI, 2);// 7
//    Serial.print(",");                 // 1 
//    Serial.print(yaw* 180.0 / PI, 2);  // 7
//    Serial.println(counter);


////////// For encoders


  if ( (millis() - startTime)/1000 > 1){
    counter = 0;
    startTime = millis();

    }else{
      counter++;
      }
          }

    delayMicroseconds(10); //Tcs waiting for another read in

}


void readEncoder()
{
    if (Serial1.available()>0)
    {

      String encoder_data = Serial1.readStringUntil('_');
//      Serial.print(encoder_data);
//      Serial.print(",");
////      Serial.println("Hey... Caption.. from serial");
      char buf[sizeof(sz)];
      encoder_data.toCharArray(buf, sizeof(buf));
      char *p = buf;
      char *str;
      int i= 0;
      while ((str = strtok_r(p, ",", &p)) != NULL){
          data_recv[i] = str;
//          Serial.println(str);
//        Serial.println(float(data_recv[i]));
          i++;
      }
    }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
