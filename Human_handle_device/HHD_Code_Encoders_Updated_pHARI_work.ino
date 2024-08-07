#include <SoftwareSerial.h>

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int analog_read       = 0;  // value read from the pot
int arm_or_disarm     = 0;  // 0 means disarm 1 means arm

// For angular rates of cable
float theta_p = 0;
float phi_p   = 0;

float time_step_ini;

/////// Encoder 1  -->  pitch (theta)
/////// Encoder 2  -->  roll  (phi)

// Pin definition
const PROGMEM int CSn1 = 4;   //D2
const PROGMEM int CLK1 = 0;   //D3
const PROGMEM int DO1  = 2;   //D4

const PROGMEM byte CSn2 = 14;   //D5
const PROGMEM byte CLK2 = 12;   //D6
const PROGMEM byte DO2  = 13;   //D7

void setup()
{
  Serial.begin(115200);
 
  pinMode(CSn1, OUTPUT);
  pinMode(CLK1, OUTPUT);
  pinMode(DO1, INPUT);

  pinMode(CSn2, OUTPUT);
  pinMode(CLK2, OUTPUT);
  pinMode(DO2, INPUT);

  // Let's start here
  digitalWrite(CLK1, HIGH);
  digitalWrite(CSn1, HIGH);

  digitalWrite(CLK2, HIGH);
  digitalWrite(CSn2, HIGH);
}

void readSensor()
{
 
  int dataOut1 = 0;
  int dataOut2 = 0;
 
  theta_p = 0;
  phi_p = 0;

//// Reading from encoder ////

  digitalWrite(CSn1, LOW);
  digitalWrite(CSn2, LOW);
  delayMicroseconds(1); //Waiting for Tclkfe

  // Passing 10 times, from 0 to 9
  for(int x=0; x<12; x++)
  {
  digitalWrite(CLK1, LOW);
  digitalWrite(CLK2, LOW);
  delayMicroseconds(50); //Tclk/2
  digitalWrite(CLK1, HIGH);
  digitalWrite(CLK2, HIGH);
  delayMicroseconds(50); //Tdo valid, like Tclk/2
  dataOut1 = (dataOut1 << 1) | digitalRead(DO1); //Shift all the entering data to the left and past the pin state to it. 1e bit is MSB
  dataOut2 = (dataOut2 << 1) | digitalRead(DO2);
  }

  digitalWrite(CSn1, HIGH);
  digitalWrite(CSn2, HIGH);

/////////////////////////////////
if (analog_read > 500){
  arm_or_disarm = 1;
  }
  
if (analog_read < 500 ){
  arm_or_disarm = 0;
  }

//  Serial.print(dataOut1);
//  Serial.print(",");
//  Serial.print(dataOut2);
//  Serial.print(",");
//  Serial.println(arm_or_disarm);

  int offset_encoder_1 = 1768;
  int offset_encoder_2 = 1915;

  int offsetted_encoder_1 = dataOut1 - offset_encoder_1;   
  int offsetted_encoder_2 = dataOut2 - offset_encoder_2;   
//  Serial.println(offsetted_encoder_1);

  phi_p   = (float) (offsetted_encoder_2 * 0.0878906);
  theta_p = (float) (-offsetted_encoder_1 * 0.0878906);

//  Serial.print(offsetted_encoder_1);
//  Serial.print(",");
//  Serial.println(offsetted_encoder_2);

//  Serial.print(phi_p);
//  Serial.print(",");
//  Serial.println(theta_p);

  int phi_p_scaled    = 50000 - (phi_p*100);
  int theta_p_scaled  = 50000 - (theta_p*100);

  String temp1 = String(phi_p_scaled);
  String temp2 = String(theta_p_scaled);
  String temp4 = String(arm_or_disarm);

  String temp3 = "," + temp1 + "_" + temp2 + "_" + temp4 +  "/";
  char attitude[16];
  temp3.toCharArray(attitude, 16);
  Serial.println(attitude);
}

void loop()
{
  
  analog_read = analogRead(analogInPin);
  readSensor();
  
  delayMicroseconds(10); //Tcs waiting for another read in
 
}
