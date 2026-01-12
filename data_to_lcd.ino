#include <LiquidCrystal.h>
#include <Servo.h>
#include <dht11.h>
#include "startup_messages.h"

// ESP32 Pin Assignments
#define DHT11PIN 32
#define SERVO_PIN 33
#define RELAY_PIN 25
#define AC_MODE_BTN 26
#define HEAT_MODE_BTN 27
#define TEMP_UP_BTN 14
#define TEMP_DOWN_BTN 12

dht11 DHT11;

// Servo and relay control
Servo ventServo;
const int SERVO_OPEN = 90;      // Servo angle to open flaps
const int SERVO_CLOSED = 0;     // Servo angle to close flaps

// Servo and temperature state variables
int setTemperature = 72;  // Default set temperature in Fahrenheit
bool flapsOpen = false;   // Flap open/closed state
bool relayActive = false; // Relay on/off state

// Low power optimization
unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 1000;  // Update display every 1 second for low power

//Pin assignments for SainSmart LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

//
//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
        return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
        return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

DFR_Key keypad;

int localKey = 0;
String keyString = "";
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // Debounce delay in milliseconds
                 
void setup() 
{ 
  lcd.begin(16, 2);
  
  // Initialize button pins
  pinMode(AC_MODE_BTN, INPUT_PULLUP);
  pinMode(HEAT_MODE_BTN, INPUT_PULLUP);
  pinMode(TEMP_UP_BTN, INPUT_PULLUP);
  pinMode(TEMP_DOWN_BTN, INPUT_PULLUP);
  
  // Initialize servo and relay
  ventServo.attach(SERVO_PIN);
  ventServo.write(SERVO_CLOSED);  // Start with flaps closed
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // Start with relay off
  
  randomSeed(analogRead(0));  // Seed random number generator
  
  // Display 10 random startup messages
  for (int i = 0; i < 10; i++) {
    int randomIndex = random(messageCount);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(startupMessages[randomIndex]);
    delay(800);
  }
  
  // Final boot message
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Let The Games");
  lcd.setCursor(0,1);
  lcd.print("Begin :)");
  delay(2000);
  lcd.clear();
}

void handleButtons() 
{
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPress < debounceDelay) return;
  
  if (digitalRead(AC_MODE_BTN) == LOW) {
    // Open flaps and activate relay for cooling
    ventServo.write(SERVO_OPEN);
    digitalWrite(RELAY_PIN, HIGH);
    flapsOpen = true;
    relayActive = true;
    lastButtonPress = currentTime;
  }
  else if (digitalRead(HEAT_MODE_BTN) == LOW) {
    // Close flaps and deactivate relay
    ventServo.write(SERVO_CLOSED);
    digitalWrite(RELAY_PIN, LOW);
    flapsOpen = false;
    relayActive = false;
    lastButtonPress = currentTime;
  }
  else if (digitalRead(TEMP_UP_BTN) == LOW) {
    setTemperature++;
    if (setTemperature > 85) setTemperature = 85;  // Max 85F
    lastButtonPress = currentTime;
  }
  else if (digitalRead(TEMP_DOWN_BTN) == LOW) {
    setTemperature--;
    if (setTemperature < 60) setTemperature = 60;  // Min 60F
    lastButtonPress = currentTime;
  }
}

String getFlapsStatus() {
  return flapsOpen ? "FLAPS:OPEN" : "FLAPS:CLOSED";
}

void loop() 
{ 
  int chk = DHT11.read(DHT11PIN);
  handleButtons();
  
  double currentTempF = Fahrenheit(DHT11.temperature);
  
  // Update display at reduced interval for low power
  unsigned long currentTime = millis();
  if (currentTime - lastDisplayUpdate >= displayUpdateInterval) {
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print(getFlapsStatus());
    lcd.setCursor(0, 1);
    lcd.print("Set:");
    lcd.setCursor(5, 1);
    lcd.print(setTemperature);
    lcd.print("F ");
    lcd.setCursor(9, 1);
    lcd.print("Now:");
    lcd.setCursor(13, 1);
    lcd.print(currentTempF, 0);
    lcd.print("F");
    
    lastDisplayUpdate = currentTime;
  }
  
  delay(50);  // Reduced delay for low power mode
}
