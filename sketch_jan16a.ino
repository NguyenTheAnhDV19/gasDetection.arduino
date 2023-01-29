/*
  this program is used for touchless bin project
  connect the motor to pin D4

  for the HC_SR04 sensor use the pin D7 for trigger and pin D6 for echo
*/

#include<Arduino.h>
#include<HC_SR04.hpp>
#include<LCD.hpp>

#define trigger 7
#define echo 6
#define motor 4

SR04 sr04(trigger,echo);
LCD lcd(0x27);

void motorController(LCD* lcd, int distance);

/*-----------------------------------------------------------------------------setup-------------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(9600);
  sr04.init();
  lcd.init();
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);

  lcd.setCursor(0,0);
  lcd.print("WELCOME");
  delay(2000);
  lcd.setCursor(1, 0);
  lcd.print("HC_SR04_PROJECT");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 4);
  lcd.print("AUTO BIN ");
}

/*-----------------------------------------------------------------------------loop---------------------------------------------------------------------------------------------*/
void loop()
{
  sr04.get_data();
  int distance = sr04.result();
  motorController(&lcd,distance);
}

void motorController(LCD* lcd, int distance){
  if(distance <=25)
  {
    lcd->setCursor(1,6);
    lcd->print("-OPEN");
    digitalWrite(motor,HIGH);
    delay(3000);
  }else{
    lcd->setCursor(1, 6);
    lcd->print("CLOSE");
    digitalWrite(motor,LOW);
  }
}
