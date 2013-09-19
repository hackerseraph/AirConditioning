#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include <dht11.h>


#define DHT11PIN 2
#define DHT11PINDOS 3

dht11 DHT11;
dht11 DHT11DOS;

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
                 
void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Warming UP...");
  delay(2000);
  lcd.clear();
  lcd.print("Chasing Zombies");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Let The Games");
  lcd.setCursor(0,1);
  lcd.print("Begin :)");
  delay(3000);
  lcd.clear();
}

void loop() 
{ 
  
  int chk = DHT11.read(DHT11PIN);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Current Temp F");
  lcd.setCursor(0, 1);
  lcd.print(Fahrenheit(DHT11.temperature), 2);
  lcd.setCursor(9,1);
  lcd.print("Degrees");
  delay(2000);
}
