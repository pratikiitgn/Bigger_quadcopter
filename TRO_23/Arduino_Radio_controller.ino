unsigned long timer[5];
byte last_channel[4];
int input[4];

#define RCPin_ch5 2
#define RCPin_ch6 7

int ch5;
int ch6;

void setup()
{
PCICR |= (1 << PCIE0);
PCMSK0 |= (1 << PCINT0);
PCMSK0 |= (1 << PCINT1);
PCMSK0 |= (1 << PCINT2);
PCMSK0 |= (1 << PCINT3);

pinMode(RCPin_ch5, INPUT);
pinMode(RCPin_ch6, INPUT);

Serial.begin(19200);
}

ISR(PCINT0_vect)
{
timer[0] = micros();

///////////// Channel 1
/////////////
if (last_channel[0] == 0 && PINB & B00000001)
{
last_channel[0] = 1;
timer[1] = timer[0];
}
else if (last_channel[0] == 1 && !(PINB & B00000001))
{
last_channel[0] = 0;
input[0] = timer[0] - timer[1];
}

///////////// Channel 2
/////////////
if (last_channel[1] == 0 && PINB & B00000010)
{
last_channel[1] = 1;
timer[2] = timer[0];
}
else if (last_channel[1] == 1 && !(PINB & B00000010))
{
last_channel[1] = 0;
input[1] = timer[0] - timer[2];
}

///////////// Channel 3
/////////////
if (last_channel[2] == 0 && PINB & B00000100)
{
last_channel[2] = 1;
timer[3] = timer[0];
}
else if (last_channel[2] == 1 && !(PINB & B00000100))
{
last_channel[2] = 0;
input[2] = timer[0] - timer[3];
}

///////////// Channel 4
/////////////
if (last_channel[3] == 0 && PINB & B00001000)
{
last_channel[3] = 1;
timer[4] = timer[0];
}
else if (last_channel[3] == 1 && !(PINB & B00001000))
{
last_channel[3] = 0;
input[3] = timer[0] - timer[4];
}
}

void publish()
{
Serial.print(input[0]);
Serial.print(",");
Serial.print(input[1]);
Serial.print(",");
Serial.print(input[2]);
Serial.print(",");
Serial.print(input[3]);
Serial.flush();
}

void loop()
{
publish();

  ch5 = pulseIn(RCPin_ch5, HIGH);
  ch6 = pulseIn(RCPin_ch6, HIGH);
  
  Serial.print(",");
  Serial.print(ch5);
  Serial.print(",");
  Serial.println(ch6);

 delay(5);
}
