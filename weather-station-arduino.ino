/*
   Weather Station for Arduino
   Compatible with:
   1. SparkFun Weather Sheild (https://www.sparkfun.com/products/12081)
   2. MP3115A2 pressure sensor breakout + HTU21D humidity sensor breaout + TEMT6000 light sensor breakout
   
   Yu-Sung Chang
*/

#include <LiquidCrystal.h>
#include <Wire.h> //I2C needed for sensors
#include "MPL3115A2.h" //Pressure sensor
#include "HTU21D.h" //Humidity sensor

// For OSEPP-LCD-01
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// For other LCD
//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

MPL3115A2 myPressure;
HTU21D myHumidity;

#define CHAR_TEMP byte(0) // Character for temperature icon
byte temp_icon[8] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
};

#define CHAR_HUMID byte(1) // Character for humidity icon
byte humid_icon[8] = {
  B00100,
  B01010,
  B01010,
  B10001,
  B11111,
  B10001,
  B01110,
};

#define CHAR_DEG byte(2) // Character for degree symbol
byte degree_icon[8] = {
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

#define CHAR_LIGHT byte(3) // Character for light icon
byte light_icon[8] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,
  B01110,
};

// Status LED pins for Weather Shield
// (Comment it out if you are using discrete breakouts)
const byte STAT1 = 7;
const byte STAT2 = 8;

// analog I/O pins for light sensor
const byte LIGHT = A1;

// Refresh rate
#define REFRESH_RATE 5000

void setup()
{
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Set up for LCD special characters
  lcd.createChar(CHAR_TEMP, temp_icon);
  lcd.createChar(CHAR_HUMID, humid_icon);
  lcd.createChar(CHAR_DEG, degree_icon);
  lcd.createChar(CHAR_LIGHT, light_icon);
  lcd.clear();

  pinMode(STAT1, OUTPUT); //Status LED Blue
  pinMode(STAT2, OUTPUT); //Status LED Green
  
  pinMode(LIGHT, INPUT); // Light sensor I/O

  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

  //Configure the humidity sensor
  myHumidity.begin();
}

float tempC;
float tempF;
float humid;
int light;

void loop()
{
  digitalWrite(STAT1, HIGH); //Blink stat LED

  myPressure.readPressure();
  humid = myHumidity.readHumidity();
  tempC = myPressure.readTemp();
  tempF = myPressure.readTempF();
  light = analogRead(LIGHT);
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(CHAR_TEMP);

  if ((tempF < 100) && (tempF >= 0))
    lcd.write(" ");

  lcd.print(tempF);
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.write(CHAR_DEG);
  lcd.write("F");
  lcd.write(" ");
  
  lcd.print(tempC);
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.write(CHAR_DEG);
  lcd.write("C");
  
  lcd.setCursor(0, 1);
  lcd.write(CHAR_HUMID);
  if (humid < 100)
    lcd.write(" ");
    
  lcd.print(humid);
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.print("%  ");

  lcd.write(CHAR_LIGHT);
  lcd.write(" ");
  lcd.print(light);

  digitalWrite(STAT1, LOW); //Turn off stat LED
  
  delay(REFRESH_RATE);
}
