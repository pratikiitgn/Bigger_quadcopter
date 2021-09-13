/*
 * rosserial Publisher Example
 * Prints "hello world!"
 * This intend to connect to a Wifi Access Point
 * and a rosserial socket server.
 * You can launch the rosserial socket server with
 * roslaunch rosserial_server socket.launch
 * The default port is 11411
 *
 */

 /*
  * Command
  * first change the IP address in the arduino code
  * $roslaunch rosserial_server socket.launch     (terminal 1)
  * $rostopic list                                (terminal 2)
  * $rostopic echo chatter                        (terminal 3)
  * $rosrun big_quadcopter CAM_device_calibration_with_ros_vicon.py (t4)
  * $roslaunch vicon_bridge vicon.launch
  */


 /*
  * Differnt between Example of Esp8266 and EspUSB32
 * ESP8266 --> #include <ESP8266WiFi.h>
 * EPS32 ----> #include <WiFi.h>
 */


#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <SoftwareSerial.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

// For angular rates of cable
float theta_p = 0;
float phi_p   = 0;
float theta_p_old = 0;
float phi_p_old   = 0;

float time_step   = 1800;
float time_step_ini;

// For reading pixhawk data
//SoftwareSerial s(3, 1);
//const unsigned int arraySize = 10;
//char quadAttitude[arraySize];// = "qwertyuio";

// Set the rosserial socket server IP address
// This need to change 
IPAddress server(192,168,137,201);

// Initialize the publisher
ros::NodeHandle nh;
geometry_msgs::Twist msg;
//std_msgs::String msg2;
ros::Publisher EncoderData("Encoder_feedback", &msg);
//ros::Publisher PxhawkData("Pixhawk_feedback", &msg2);

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
//  s.begin(57600);
 
//  // GPIO 1 (TX) swap the pin to a GPIO.
//  pinMode(1, FUNCTION_3);
//  // GPIO 3 (RX) swap the pin to a GPIO.
//  pinMode(3, FUNCTION_3);
 
  // FOR OTA updates
  ArduinoOTA.begin();

  // Connect the ESP8266 the wifi AP
  WiFi.begin("IDGR", "psshcrlab");    // ssid, password
 
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

  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(server, 11411);
  nh.initNode();
  nh.advertise(EncoderData);
//  nh.advertise(PxhawkData);
}


void loop()
{
  ArduinoOTA.handle();
 
  if (nh.connected())
  {
  readSensor();
//  readPixhawk();
  }

  // Without ROS
//  readSensor();

//  Serial.println(String(phi_p) + " , " + String(theta_p));
 
  nh.spinOnce();
  delayMicroseconds(10); //Tcs waiting for another read in
}

void readPixhawk()
{
//  for (int i = 0; i < arraySize; i++)
//  {
//  quadAttitude[i] = s.read();
//  }
//  Serial.println(quadAttitude);
//  quadAttitude[0] = s.read();
//  msg2.data = quadAttitude;
//  PxhawkData.publish(&msg2);
}

void readSensor()
{
  time_step = micros() - time_step_ini ;
  time_step_ini = micros();
 
  int dataOut1 = 0;
  int dataOut2 = 0;
 
  theta_p = 0;
  float theta_p_dot = 0;

  phi_p = 0;
  float phi_p_dot = 0;

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

  int offset_encoder_1 = 2495;
  int offset_encoder_2 = 1633;

  int offsetted_encoder_1 = dataOut1 - offset_encoder_1;   
  int offsetted_encoder_2 = dataOut2 - offset_encoder_2;   

  float E_1_phi =  497;   // Encoder position 1
  float V_1_phi =  41.67; // Encoder position 2
  float E_2_phi =  35;    // Vicon position 1
  float V_2_phi =  0.67;  // vicon position 2
 
  float E_1_theta = 490 ; // Encoder position 1
  float V_1_theta = 40 ;  // Encoder position 2
  float E_2_theta = 20 ;  // Vicon position 1
  float V_2_theta = 0.3 ; // vicon position 2
 
  float steadyStateError_thetap = 0.5;
  float steadyStateError_phip   = 2.5;
 
  phi_p   = (float)(V_1_phi+((offsetted_encoder_1 - E_1_phi) * (V_2_phi - V_1_phi) / (E_2_phi - E_1_phi))) - steadyStateError_phip;
  theta_p = (float)(V_1_theta+((offsetted_encoder_2 - E_1_theta) * (V_2_theta - V_1_theta) / (E_2_theta - E_1_theta))) - steadyStateError_thetap;



  // Calculate anlge from encoder reading
//  theta_p = (float)(dataOut1 - 2185) * -66.25 / 686;
//  phi_p = (float)(dataOut2 - 1111) * -57.3 / 570;
    
  theta_p_dot  = (theta_p - theta_p_old) / time_step * 1000000;
  phi_p_dot = (phi_p - phi_p_old) / time_step * 1000000;

  theta_p_old = theta_p;
  phi_p_old   = phi_p;
 
//  analogWrite(roll_px, pwm_valueRoll);
//  analogWrite(pitch_px, pwm_valuePitch);

  msg.linear.x = theta_p;
  msg.linear.y = phi_p;
 
  msg.angular.x  = theta_p_dot;
  msg.angular.y  = phi_p_dot;
  msg.angular.z  = time_step;
  EncoderData.publish(&msg);
}
