#include <Wire.h>
int offset =20;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
float h;
 float t;
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin 4
DHT dht(dht_dpin, DHTTYPE);
#include <SoftwareSerial.h>
//#include <dht11.h>
#define RX 2
#define TX 3


String AP = "byp";       // AP NAME
String PASS = "Byp12345678"; // AP PASSWORD
String API = "THNFXYWDMTGUG3SW";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;  
int valSensor = 1;

SoftwareSerial esp8266(RX,TX);

void setup()
{
  Serial.begin(9600);
   pinMode(A0,INPUT);
   pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
      pinMode(12,OUTPUT);
       pinMode(8,OUTPUT);
    digitalWrite(11,1);
    digitalWrite(12,1);
        digitalWrite(10,1);
           digitalWrite(8,0);
  lcd.init();
         dht.begin();
 lcd.backlight();
    lcd.setCursor(1,0); //(IOT) Based Weather Monitoring system
  lcd.print("IOT BASED BMS ");
  delay(2000);
  lcd.clear();


  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");


}


void loop()
{
 

 lcd.print("TEMPERATURE:");
lcd.setCursor(13,0);
lcd.print(getTemperatureValue());
lcd.setCursor(0,1);
 lcd.print("HUMIDITY:");
lcd.setCursor(13,1);
lcd.print(getHumidityValue());
delay(2000);
lcd.clear();

lcd.print("VOLTAGE:");
lcd.setCursor(8,0);
lcd.print(bpm());
delay(2000);
lcd.clear();
          String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue()+"&field3="+bpm();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

     
 
   
}


String getTemperatureValue(){
 h = dht.readHumidity();
  t = dht.readTemperature();  
     
   
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t);
    Serial.print("C  ");

    if(t>=45)
    {
     
        digitalWrite(10,0);
         digitalWrite(8,1);
     delay(3000);
       digitalWrite(8,0);
         delay(3000);
   
               delay(2000);
lcd.clear();


lcd.setCursor(0,0);
 lcd.print("TEMP HIGH");
 
delay(2000);
lcd.clear();
    }
    if(t<=45)
    {
 
        digitalWrite(10,1);
   
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t);
    Serial.print("C  ");
    }
    if(t<=5)
    {
 
        digitalWrite(11,0);
   
    }
     if(t>=10)
    {
 
        digitalWrite(11,1);
   
    }
   return String(t);
 
}


String getHumidityValue(){
 h = dht.readHumidity();
  t = dht.readTemperature();  
     
   
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t);
    Serial.print("C  ");

   
   return String(h);
 
}

String bpm(){

 int volt = analogRead(A0);
  double voltage = map(volt,0,1023, 0, 2526) + offset;
 
  voltage /=120;
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.println("v");
 
   if(voltage<=5)
   {
   
   

    digitalWrite(12,0);
      digitalWrite(8,1);
      delay(3000);
          digitalWrite(8,0);
             delay(3000);
        lcd.clear();


lcd.setCursor(3,0);
 lcd.print("VOLTAGE LOW ");
delay(2000);
lcd.clear();
     
    delay(3000);
 
   
        digitalWrite(13,0);
           delay(3000);
   }
if(voltage>=10)
   {
   
   

    digitalWrite(12,1);
   
       
   }
 
   return String(voltage);
 
}



void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
 
    countTimeCommand++;
  }
 
  if(found == true)
  {
    Serial.println("Conn");
    countTrueCommand++;
    countTimeCommand = 0;
  }
 
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
 
  found = false;
 }