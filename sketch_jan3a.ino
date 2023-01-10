/*
-----------------------------------------dht11-----------------------------------
connect the dht11 sensor with data pin  pin 4;
connect the motor to pin 2;
display it on LCD;
if the temperature >28 then start to run the motor ;
the greater the temperature is, the faster motor run;

----------------------------------------mq2--------------------------------------
connect the data pin of mq2 to pin A0;
connect the buzzer to pin 7;
display on LCD;
if the value of sensor is greater than 30% than ring the bell;
display on the lcd : "gas detection!!!"
*/

#include<Arduino.h>
#include<DHT11.hpp>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

const int buzzer =7;
const int motor = 2;

class DHT
{
private:
  LiquidCrystal_I2C* lcd;
  DHT11* dht;
  int motor;
  void DHT_fanControl(int temp);

public:
  DHT(LiquidCrystal_I2C* r_lcd,DHT11* r_dht,int motor);
  void DHT_main();
};

class MQ2
{
public:
  MQ2(int _pin,LiquidCrystal_I2C* r_lcd);
  void MQ2_main();

private:
  int pin;
  LiquidCrystal_I2C* lcd;
  void buzzer(bool con);
};

/*-------------------------------------------------------------------create object----------------------------------------------------------------------- */
LiquidCrystal_I2C lcd(0x27,16,2);
DHT11 dht(4);
DHT dht_sensor(&lcd,&dht,motor);
MQ2 mq2_sensor(&buzzer,&lcd);

void setup()
{
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(motor,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("0");
}

void loop()
{
  dht_sensor.DHT_main();
  mq2_sensor.MQ2_main();
}

/*----------------------------------------------------------temp-humid sensor------------------------------------------------------------------------------*/

DHT::DHT(LiquidCrystal_I2C* r_lcd,DHT11* r_dht,int p_motor)
:lcd(r_lcd),dht(r_dht),motor(p_motor){}

void DHT::DHT_main()
{
  dht->get_data();
  unsigned long time = millis();
  int temp = dht->get_temperature(true);
  int humid = dht->get_humidity();
  lcd->setCursor(0,0);
  lcd->print("temp: ");
  lcd->print(temp);
  lcd->setCursor(0,1);
  lcd->print("humid: ");
  lcd->print(humid);
  DHT_fanControl(temp);
  Serial.println(millis()-time);
}

void DHT::DHT_fanControl(int temp)
{
  if(temp<27){
    digitalWrite(motor,LOW);
  }else if(temp>=27 && temp<=30){
    digitalWrite(motor,HIGH);
    delay(400);
  }else if(temp>=30 && temp<=33){
    digitalWrite(motor,HIGH);
    delay(800);
  }
  digitalWrite(motor,LOW);
}

/*------------------------------------------------------------MQ2------------------------------------------------------------------------------------- */
MQ2::MQ2(int _pin,LiquidCrystal_I2C* r_lcd)
: pin(_pin),lcd(r_lcd){}

void MQ2::MQ2_main()
{
  int value = analogRead(pin);
  value = map(value,1024,0,100,0);
  if(value>=30){
    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print("Gas detection!!!");
    buzzer(true);
    while(value>=30){};
  }
  buzzer(false);
  lcd->setCursor(9,0);
  lcd->print("gas: ");
  lcd->print(value);
}

void MQ2::buzzer(bool con)
{
  if(con && !digitalRead(pin))
  {
    digitalWrite(pin,HIGH);
  }else if(!con && digitalRead(pin)){
    digitalWrite(pin,LOW);
  }
}