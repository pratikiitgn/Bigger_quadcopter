// For angular rates of cable
float theta_p = 0.0;
float phi_p   = 0.0;

// Pin definition

const PROGMEM int CSn1 = 2;   //D2
const PROGMEM int CLK1 = 3;   //D3
const PROGMEM int DO1  = 4;   //D4

const PROGMEM byte CSn2 = 5;   //D5
const PROGMEM byte CLK2 = 6;   //D6
const PROGMEM byte DO2  = 7;   //D7 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Serial1.begin(230400);

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

  int offset_encoder_1 = 1988;
  int offset_encoder_2 = 1911;

//  String temp1 = String(offset_encoder_1);
//  Serial.print(dataOut1);
//  Serial.print(",");
//  Serial.println(dataOut2);

  int offsetted_encoder_1 = dataOut1 - offset_encoder_1;   
  int offsetted_encoder_2 = dataOut2 - offset_encoder_2;   

  theta_p  =  -(float) (offsetted_encoder_2 * 0.0878906);
  phi_p     = -(float) (offsetted_encoder_1 * 0.0878906);
//  Serial.print(phi_p);
//  Serial.print(",");
//  Serial.println(theta_p);
  int phi_p_scaled    = 50000 + (phi_p*100);
  int theta_p_scaled  = 50000 + (theta_p*100);

/// Previous code  
  String temp1 = String(phi_p_scaled);
  String temp2 = String(theta_p_scaled);
  String temp3 = "," + temp1 + "_" + temp2 + "/";
//  String temp3 = ",12345_67890/";
  char attitude[14];
  temp3.toCharArray(attitude, 14);
  Serial.println(attitude);
  Serial1.println(attitude);

}

void loop() {

      readSensor();
      delay(10); //Tcs waiting for another read in

}
