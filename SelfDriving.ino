
#include<Servo.h>
Servo servo;
int dcmotor_pwm=11;  //mosfete pwm uygulanan pindir.
int ch1;  //throtle
int pwm_value=0;  //motorun dönme hızıdır.
int angle=90;  //servo motorun açı değeridir.
uint8_t data;
int value;

void setup() 
{
  PwmFrequencySetting(dcmotor_pwm);
  pinMode(5, INPUT);
  pinMode(dcmotor_pwm,OUTPUT);
  servo.attach(9);
  servo.write(angle);
  Serial.begin(9600);
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) 
{
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() 
{
  ch1 = pulseIn(5, HIGH, 30000);
  delayMicroseconds(20);

  pwm_value=map(ch1,985,1975,0,255);
  pwm_value=constrain(pwm_value,0,255);
  analogWrite(dcmotor_pwm,pwm_value);
  
  if(Serial.available()>0)
  {
    data=Serial.read();
    if(data=='0')
    {
      value=0;  
    }
    else if(data=='1')
    {
      value=1;  
    }
    else if(data=='2')
    {
      value=2;  
    }
    else if(data=='3')
    {
      value=3;  
    }
    else if(data=='4')
    {
      value=4;  
    }
    angle=map(value,0,4,45,135);
    angle=constrain(angle,45,135);
    servo.write(angle);
  }
}

void PwmFrequencySetting(int pin) 
{
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) 
  {
    if(pin == 5 || pin == 6) 
    {
      TCCR0B = TCCR0B & 0b11111000 | 0x01;
    } 
    else 
    {
      TCCR1B = TCCR1B & 0b11111000 | 0x01;      //31 KHz
      //TCCR1B = TCCR1B & B11111000 | B00000010;  //4 KHz
      //TCCR1B = TCCR1B & B11111000 | B00000011;  //500 Hz
      //TCCR1B = TCCR1B & B11111000 | B00000100;  //122 Hz
      //TCCR1B = TCCR1B & B11111000 | B00000101;  //30 Hz
    }
  }
}
