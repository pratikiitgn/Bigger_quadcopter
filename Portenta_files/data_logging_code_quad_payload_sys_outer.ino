#include <SD.h>
#include <SPI.h>

// Class Instances____________________________________________________________________________________________________________
File csvFile;
//============================================================================================================================
// Variable Initialization____________________________________________________________________________________________________
int sdCS = 7;                             // Chip Select Pin
int count = 0;
#define FILE_BASE_NAME "QUAD"
char fileName[] = FILE_BASE_NAME "00.csv";
float currTime;
char sz[116];
//char sz[119];
String data_recv[18];
//____________________________________________________________________________________________________________________________
void setup()
{
  Serial.begin(2000000);
  Serial1.begin(2000000);

//  while(!Serial){
//    ;
//  }
  
  //SD Card Setup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(sdCS, OUTPUT);                                          // Enable CS Pin for SD Card SPI communication
  Serial.print("Initializing SD card...");
  if (!SD.begin(sdCS)) {
    Serial.println("Card failed, or not present");
    // Blink Yellow ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    digitalWrite(LEDR,LOW);                                        // Blink Portenta Red Light
    digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
    digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
    float startTime = millis();
    while (1);
  }
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  //char fileName[13] = FILE_BASE_NAME "00.csv";
  Serial.println("card initialized.");
  while (SD.exists(fileName))
  {
      if (fileName[BASE_NAME_SIZE + 1] != '9')
      {
        fileName[BASE_NAME_SIZE + 1]++;
      }
      else if (fileName[BASE_NAME_SIZE] != '9')
      {
        fileName[BASE_NAME_SIZE + 1] = '0';
        fileName[BASE_NAME_SIZE]++;
      }
      else
      {
        Serial.println("Can't create file name");
        // Blink Yellow ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        digitalWrite(LEDR,LOW);                                        // Blink Portenta Red Light
        digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
        digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
      }
  }
  csvFile = SD.open(fileName, FILE_WRITE);
  csvFile.print("currTime");       csvFile.print(",");

    csvFile.print("Arm_disarm");           csvFile.print(",");
  
    csvFile.print("X");           csvFile.print(",");
    csvFile.print("Y");           csvFile.print(",");
    csvFile.print("Z");           csvFile.print(",");

    csvFile.print("ph");           csvFile.print(",");
    csvFile.print("th");           csvFile.print(",");
    csvFile.print("ps");           csvFile.print(",");

    csvFile.print("ph_h");           csvFile.print(",");
    csvFile.print("th_h");           csvFile.print(",");
    csvFile.print("ps_h");           csvFile.print(",");
    csvFile.print("ps_h_dot");       csvFile.print(",");
    csvFile.print("z_h_dot");        csvFile.print(",");

    csvFile.print("PWM1");           csvFile.print(",");
    csvFile.print("PWM2");           csvFile.print(",");
    csvFile.print("PWM3");           csvFile.print(",");
    csvFile.print("PWM4");           csvFile.print(",");

    csvFile.print("ph_p");           csvFile.print(",");
    csvFile.print("th_p");           csvFile.print(",");

    csvFile.println();
    csvFile.close();
}
void loop()
{
  count = count + 1;
  csvFile = SD.open(fileName, FILE_WRITE);
  Serial.println("Hi");
  if(count%2 == 1)
  {
    // Blink Cyan ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    digitalWrite(LEDR,HIGH);                                        // Blink Portenta Red Light
    digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
    digitalWrite(LEDB,LOW);                                         // Blink Portenta Blue Light
  }
  else{
    // Blink Green ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    digitalWrite(LEDR,HIGH);                                        // Blink Portenta Red Light
    digitalWrite(LEDG,LOW);                                         // Blink Portenta Green Light
    digitalWrite(LEDB,HIGH);                                         // Blink Portenta Blue Light
  }
  float startTime = millis();
  while ( ((millis() - startTime)/1000) <= 10.0)
  {
    currTime = (millis() - startTime);
//           Serial.println("Hey... Caption..");

    if (Serial1.available()>0)
    {

      String pixhawk_data = Serial1.readStringUntil('_');
      Serial.println(pixhawk_data);
////      Serial.println("Hey... Caption.. from serial");
      char buf[sizeof(sz)];
      pixhawk_data.toCharArray(buf, sizeof(buf));
      char *p = buf;
      char *str;
      int i= 0;
      while ((str = strtok_r(p, ",", &p)) != NULL){
          data_recv[i] = str;
//          Serial.println(str);
          i++;
      }
      Write_SDcard();
    }
  }

//  Serial.println("Hello");
  csvFile.close();

// To send data to Pixhawk

//  sending_data_pixhawk();

}

//void sending_data_pixhawk()
//{
//
//  Serial1.println(",50000_50000/");
//  
//}

void Write_SDcard()
{
  if (csvFile){
    csvFile.print(String(currTime));       csvFile.print(",");
    
    csvFile.print(data_recv[0]);           csvFile.print(",");

    csvFile.print(data_recv[1]);           csvFile.print(",");
    csvFile.print(data_recv[2]);           csvFile.print(",");
    csvFile.print(data_recv[3]);           csvFile.print(",");

    csvFile.print(data_recv[4]);           csvFile.print(",");
    csvFile.print(data_recv[5]);           csvFile.print(",");
    csvFile.print(data_recv[6]);           csvFile.print(",");

    csvFile.print(data_recv[7]);           csvFile.print(",");
    csvFile.print(data_recv[8]);           csvFile.print(",");
    csvFile.print(data_recv[9]);           csvFile.print(",");
    csvFile.print(data_recv[10]);           csvFile.print(",");
    csvFile.print(data_recv[11]);           csvFile.print(",");

    csvFile.print(data_recv[12]);           csvFile.print(",");
    csvFile.print(data_recv[13]);           csvFile.print(",");
    csvFile.print(data_recv[14]);           csvFile.print(",");
    csvFile.print(data_recv[15]);           csvFile.print(",");

    csvFile.print(data_recv[16]);           csvFile.print(",");
    csvFile.print(data_recv[17]);           csvFile.print(",");

    csvFile.println(); //End of Row move to next row
  }
}
