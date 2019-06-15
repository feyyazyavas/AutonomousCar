
#include<Servo.h>
#include<math.h>
Servo servo;
int dcmotor_pwm=11;  //mosfete pwm uygulanan pindir.
int ch1;  //throtle
int ch2;  //servo
int pwm_value=0;  //motorun dönme hızıdır.
int angle=90;  //servo motorun açı değeridir.
double angle_state;
uint8_t data;

void setup() 
{
  PwmFrequencySetting(dcmotor_pwm);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
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
  ch2 = pulseIn(6, HIGH, 30000);
  delayMicroseconds(20);

  pwm_value=map(ch1,985,1975,0,255);
  pwm_value=constrain(pwm_value,0,255);
  analogWrite(dcmotor_pwm,pwm_value);
  
  angle=map(ch2,985,1975,45,135);
  angle=constrain(angle,46,134);
  servo.write(angle);

  angle_state=mapf(angle,46,134,0,4);
  angle_state=round(angle_state);
   
  if(Serial.available()>0)
  {
    data=Serial.read();
    if(data=='1')
    {          
      Serial.println(angle_state);    
    }
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
