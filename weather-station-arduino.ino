/*
   Weather Station for Arduino
   with Weather Sheild (from SparkFun)
   
   Yu-Sung Chang
*/

#include <LiquidCrystal.h>
#include <Wire.h> //I2C needed for sensors
#include "MPL3115A2.h" //Pressure sensor
#include "HTU21D.h" //Humidity sensor

// For OSEPP-LCD-01
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// For regular LCD
//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity; //Create an instance of the humidity sensor

byte temp_icon[8] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
};

byte humid_icon[8] = {
  B00100,
  B01010,
  B01010,
  B10001,
  B11111,
  B10001,
  B01110,
};

byte celsius_icon[8] = {
  B11000,
  B11000,
  B00110,
  B01001,
  B01000,
  B01001,
  B00110,
};

byte fahrenheit_icon[8] = {
  B11000,
  B11000,
  B00111,
  B00100,
  B00111,
  B00100,
  B00100,
};

byte degree_icon[8] = {
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

// digital I/O pins
const byte WSPEED = 3;
const byte RAIN = 2;
const byte STAT1 = 7;
const byte STAT2 = 8;

// analog I/O pins
const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;
const byte WDIR = A0;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, temp_icon);
  lcd.createChar(1, humid_icon);
  lcd.createChar(2, celsius_icon);
  lcd.createChar(3, fahrenheit_icon);
  lcd.createChar(4, degree_icon);
  lcd.clear();

  pinMode(STAT1, OUTPUT); //Status LED Blue
  pinMode(STAT2, OUTPUT); //Status LED Green
  
  pinMode(WSPEED, INPUT_PULLUP); // input from wind meters windspeed sensor
  pinMode(RAIN, INPUT_PULLUP); // input from wind meters rain gauge sensor
  
  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);

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

void loop()
{
  digitalWrite(STAT1, HIGH); //Blink stat LED

  myPressure.readPressure();
  humid = myHumidity.readHumidity();
  tempC = myPressure.readTemp();
  tempF = myPressure.readTempF();
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  if ((tempF < 100) && (tempF >= 0))
    lcd.write(" ");

  lcd.print(tempF);
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.write(byte(4));
  lcd.write("F");
  lcd.write(" ");
  
  lcd.print(tempC);
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.write(byte(4));
  lcd.write("C");
  
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  if (humid < 100)
    lcd.write(" ");
  
  lcd.print(myHumidity.readHumidity());
  lcd.rightToLeft();
  lcd.write(" ");
  lcd.leftToRight();
  lcd.print(" %");

  digitalWrite(STAT1, LOW); //Turn off stat LED
  
  delay(1000);
}
