#include <Servo.h>
Servo ESC;     // create servo object to control the ESC

int flag = 0;

void setup() {
  // Attach the ESC on pin 9
  ESC.attach(9, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)
  Serial.begin(9600); 
}
void loop() {

      ESC.write(1000);    // Send the signal to the ESC

///////////////////////// for decreasing data ///////////////////////// 

//  int pwm_ = 1000;
//  if (flag == 0 )
//  {
//    delay(5000);          // wait a second so as not to send massive amounts of data
//    for (int ii = 0; ii < 1000; ii = ii + 50) {
//      pwm_ = 2000 - ii;
////      ESC.write(pwm_);    // Send the signal to the ESC
//      Serial.println(pwm_);
//      delay(2000);          // wait a second so as not to send massive amounts of data
//    };
////    ESC.write(1000);    // Send the signal to the ESC
//
//    pwm_ = 1000;
//    flag = 1;
//  }
////  ESC.write(pwm_);    // Send the signal to the ESC
//
//  Serial.println(pwm_);
///////////////////////////////////////////////////////////////////////////


///////////////////////// for increasing data /////////////////////////

  int pwm_ = 1000;
  if (flag == 0 )
  {  
    delay(5000);          // wait a second so as not to send massive amounts of data
    for (int ii = 0; ii < 1001; ii = ii + 50) {
      pwm_ = 1000 + ii;
      ESC.write(pwm_);    // Send the signal to the ESC
      Serial.println(pwm_);
      delay(2000);          // wait a second so as not to send massive amounts of data
    };
    ESC.write(1000);    // Send the signal to the ESC

    pwm_ = 1000;
    flag = 1;
  }

  ESC.write(pwm_);    // Send the signal to the ESC

  Serial.println(pwm_);

///////////////////////////////////////////////////////////////////////////




}
