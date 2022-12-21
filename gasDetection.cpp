#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Adafruit_Keypad.h>

LiquidCrystal_I2C display(0x27,16,2); // create object display

void loading(LiquidCrystal_I2C* display);
void welcome(LiquidCrystal_I2C* display);
void sensor(int* value);
void alert(int* value,LiquidCrystal_I2C* display);
void buzzer();

int value = 0;
int buzzerPin = 2;

void setup() {
  pinMode(buzzer,OUTPUT);
  
  display.init();
  display.backlight();
  loading(&display);
  welcome(&display);

  Serial.begin(9600);  
}

void loop() {
  sensor(&value,&display);
}

void loading(LiquidCrystal_I2C* display){
  display->setCursor(0,0);
  display->print("Gas detection");
  delay(1000);
  for(int i=0; i<= 10;++i){
      display->setCursor(0,1);
      display->print("loading: ");
      display->print(i);
      display->print("%");
      delay(100);
  }
  display->clear();
}

void welcome(LiquidCrystal_I2C* display){
  display->setCursor(3,0);
  display->print("Welcome");
  delay(2000);
  display->clear();
}

void sensor(int* value,LiquidCrystal_I2C* display){
  *value = analogRead(A0);
  display->setCursor(0,0);
  int value_display = map(*value,1024,0,100,0);
  alert(value_display,display);
}

void alert(int value_display,LiquidCrystal_I2C* display){
  value_display -= 6;
  if(value_display >=30){
      display->clear();
      display->setCursor(4,0);
      while(value_display>=30){
        display->setCursor(3,0);
        display->print("Aleart");
        buzzer();
    }
   }else{
    display->print("Gas: ");
    display->print(value_display);
    display->print("%");
    delay(500);
    }
}

void buzzer(){
  digitalWrite(buzzerPin,HIGH);
  delay(1000);
}
