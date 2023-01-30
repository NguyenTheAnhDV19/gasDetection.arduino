/*
  food dispenser 
  8 hrs each time/ ring bell to alarm the meal is ready/restart the timer to 00::00::30
  after 30 minutes ring bell again -> tossing out the remaining food/ restart the timer to 08::00::00
  LCD display time remaining 00::00::00
  can press to restart the process again/ bluetooth JDY 

  measure the humidity and tempeature of food and show it on lcd (2nd line)
  if the tempeature is greater than 80 F then display on lcd :"tempeature exceeded" (2nd line)

*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JDY.hpp>
#include <Timer.hpp>
#include<LCD.hpp>
#include<DHT11.hpp>

#define buzzer 1
#define dht_pin 5
#define motor 7

bool runTimer1 = 1;
bool runTimer2 =1;

SoftwareSerial device(3,4);

JDY bluetooth(&device);
Timer timer1;
Timer timer2;
DHT11 dht(dht_pin);
LCD lcd(0x27);

volatile int buttonFlag;

void button(Timer* timer1, Timer* timer2);
void restartTimer(Timer* timer);
void displayTemp(LCD* lcd, DHT11* dht);
void offBuzzer(int seconds);
void onBuzzer(int seconds);
void displayTimer2(LCD* lcd, Timer* timer2);
void displayTimer1(LCD* lcd, Timer* timer1);
void alarmTemp(LCD* lcd, DHT11* dht);
void onMotor(int seconds);
void offMotor(int seconds);

void setup() 
{
  Serial.begin(9600);
  device.begin(9600);
  timer1.init("00:00:05");
  timer2.init("00:00:05");
  lcd.init();
  pinMode(buzzer,OUTPUT);
  pinMode(motor,OUTPUT);
  attachInterrupt(2,button,CHANGE);
}

void loop() 
{
  lcd.setCursor(0, 0);
  lcd.print("Timer: ");
  bool temp =0;

  while(runTimer1){
    lcd.setCursor(0,7);
    displayTimer1(&lcd,&timer1);
    timer1.checkTimeOut() ? (runTimer1 = 0) : (runTimer1 = 1);
    
    dht.get_data();
    if(dht.get_temperature(0) <80){
      displayTemp(&lcd,&dht);
    }else{
      alarmTemp(&lcd,&dht);
    }
  }

  onBuzzer(2000);
  onMotor(2000);
  bluetooth.sendData("food ");
  restartTimer(&timer2);
  runTimer2 =1;

  temp =0;
  while(runTimer2){
    lcd.setCursor(0,7);
    displayTimer2(&lcd,&timer2);
    timer2.checkTimeOut() ? (runTimer2 = 0) : (runTimer2 = 1);
    dht.get_data();
    if(dht.get_temperature(0) <80){
      displayTemp(&lcd,&dht);
    }else{
      alarmTemp(&lcd,&dht);
    }
  }

  onBuzzer(1000);
  offBuzzer(1000);
  onBuzzer(1000);
  bluetooth.sendData("toss out food");

  runTimer1 =1;
  restartTimer(&timer1);
}

void displayTimer1(LCD* lcd, Timer* timer1)
{
  lcd->print(timer1->countDown());
}

void displayTimer2(LCD* lcd, Timer* timer2)
{
  lcd->print(timer2->countDown());
}

void onBuzzer(int seconds)
{
  digitalWrite(buzzer,HIGH);
  delay(seconds);
  offBuzzer(0);
}

void offBuzzer(int seconds)
{
  digitalWrite(buzzer,LOW);
  delay(seconds);
}

void displayTemp(LCD* lcd, DHT11* dht)
{
  lcd->setCursor(1,0);
  lcd->print("temp: ");
  lcd->print_number(dht->get_temperature(0));
  lcd->setCursor(1, 8);
  lcd->print("humid: ");
  lcd->print_number(dht->get_humidity());
}

void alarmTemp(LCD* lcd, DHT11* dht)
{
    lcd->setCursor(1,0);
    lcd->print("temp exceeded");
}

void restartTimer(Timer* timer)
{
  timer->resetAll();
}

void button(Timer* timer1, Timer* timer2)
{
  restartTimer(timer1);
  restartTimer(timer2);
}

void onMotor(int seconds)
{
  digitalWrite(motor,HIGH);
  delay(seconds);
  offMotor(0);
}

void offMotor(int seconds)
{
  digitalWrite(motor,LOW);
  delay(seconds);
}





