#include <DHT.h>


//HOME AUTOMATION                                                                                                                           by Mokesh Nagpal.  
#include <Servo.h>     //import libraries
#include <NewPing.h>   
#define sensor DHT22    //define sensor type used

const int motor = 10;
const int echoenter = 8;
const int trigenter = 9;
const int echoexit = 6;
const int trigexit = 7;
const int ac = 11;
const int servo = A5;
const int tempHum = 3;
const int li = 4;

int x=0,y=0;
float temp = 25;  
float hum = 55;
int denter=0,dexit=0;
int p=0;
int o=0;
int max_distance=400;
int fadeAmount=5;
int brightness=255;
char incomingByte='\0';
int distance_M = 0;
int m=0;
long int ta,ti=0;


NewPing sonarenter(trigenter, echoenter, max_distance);
DHT dht(tempHum,sensor);
NewPing sonarexit(trigexit, echoexit, max_distance);
Servo ser;

void setup() 
{
  dht.begin();
  pinMode(ac, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(motor, OUTPUT);    //intitialize pins as output or input
  fanoff();
  lightoff();
  gateclose();
  acoff();
  ser.attach(servo);
  Serial.begin(9600);
  ta=millis();
  delay(2000);
  tempHumSen();
}
void loop() 
{
  if (Serial.available() > 0)   
  {
    incomingByte = Serial.read(); 
    if(incomingByte!='i')
      m=1;  
    switch(incomingByte)    
    {
      case 'a':
      fan();
      break;
      case 'b':
      fanoff();
      break;
      case 'c':
      light();
      break;
      case 'd':
      lightoff();
      break;
      case 'e':
      gateopen();
      break;
      case 'f':
      gateclose();
      break;
      case 'g':
      y=x;
      x=255;
      acon();
      break;
      case 'h':
      acoff();
      break;
      case 'i':
      x=y;
      m=0;
      break;
    }
  }
  if(millis()-ta>4000)
  {
    tempHumSen();
    ta=millis();
  }
  if(m==0)
  {
    if(p>3)
    {
      acon();
    }
    else 
    {
      acoff();
    }
    readSensorenter();
    readSensorexit();
    if(denter>=0.5&&denter<=12)
    {
        ti=0;
        gateopen();
        while(1)
        {
          readSensorenter();
          readSensorexit();
          if(dexit>=0.5&&dexit<=12)
          {
            gateclose();
            light();
            fan();
            p++;
            dexit=13;
            denter=13;
            delay(1000);
            break;
          }
          if((denter<0.5||denter>12)&&ti==0)
            ti=millis();
          if((denter<0.5||denter>12)&&(millis()-ti)>5000)
          {  
            gateclose();
            ti=0;
            break;
          }
        }
    }
    if(dexit>=0.5&&dexit<=12)
    {
        ti=0;
        gateopen();
        while(1)
        {
          readSensorenter();
          readSensorexit();
          if(denter>=0.5&&denter<=12)
          {
            gateclose();
            p--;
            if(p<=0)
            {
              lightoff();
              fanoff();
              p=0;
            }
            denter=13;
            dexit=13;
            delay(1000);
            break;
          }
          if((dexit<0.5||dexit>12)&&ti==0)
            ti=millis();
          if((dexit<0.5||dexit>12)&&(millis()-ti)>5000)
          {  
            gateclose();
            ti=0;
            break;
          }
        }
    }
  }
}
void gateopen()
{
  for (int pos = 0; pos <= 90; pos += 1) 
  { 
    ser.write(pos);
  }
}
void gateclose()
{
  for (int pos = 90; pos >= 0; pos -= 1) 
  { 
    ser.write(pos);
  }
}
void fan()
{
  digitalWrite(motor,LOW);
}
void fanoff()
{
  digitalWrite(motor,HIGH);
}
void light()
{
  analogWrite(li,255);
}
void lightoff()
{
  digitalWrite(li,LOW);
}
void readSensorenter()    
{ 
  denter = sonarenter.ping_cm();
}
void readSensorexit()   
{ 
  dexit = sonarexit.ping_cm();
}
void acon()
{
  analogWrite(ac, x);
}
void acoff()
{
  digitalWrite(ac,LOW);
}
void tempHumSen()
{
  int temp1=0,hum1=0;
  hum=dht.readHumidity();
  temp=dht.readTemperature();
  x=((hum-55)/10)+(temp-24);
  x=map(x,0,40,0,255); 
}
