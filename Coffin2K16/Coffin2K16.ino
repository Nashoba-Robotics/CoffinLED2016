#include <Adafruit_NeoPixel.h>

#define PIN_HOOD_BOTTOM 22
#define PIN_HOOD_TOP 23
#define PIN_HOOD_AT_THRESHOLD 24

#define PIN_INTAKE_TOP_STOP 25
#define PIN_INTAKE_TOP 26
#define PIN_INTAKE_TOP_INTAKE 27
#define PIN_INTAKE_INTAKE_HOME 28
#define PIN_INTAKE_HOME 29
#define PIN_INTAKE_HOME_BOTTOM 30
#define PIN_INTAKE_BOTTOM 31
#define PIN_INTAKE_BOTTOM_STOP 32

#define PIN_PHOTO_1 33
#define PIN_PHOTO_2 34
#define PIN_PHOTO_3 35

#define PIN_ELEVATOR_BOTTOM_HEIGHT 36
#define PIN_ELEVATOR_TOP_HEIGHT 37
#define PIN_MOTOR_RUNNING 38

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd1(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd2(12, 11, 5, 4, 3, 2); //TODO: determine pin numbers for lcd2
LiquidCrystal actualLcd(48, 49, 50, 51, 52, 53);

#define LED_STRIP_PIN 50
#define LED_COUNT 45

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

int myPins[] = {PIN_HOOD_BOTTOM, PIN_HOOD_TOP, PIN_HOOD_AT_THRESHOLD,
PIN_INTAKE_TOP_STOP, PIN_INTAKE_TOP, PIN_INTAKE_TOP_INTAKE, 
PIN_INTAKE_INTAKE_HOME, PIN_INTAKE_HOME, PIN_INTAKE_HOME_BOTTOM,
PIN_INTAKE_BOTTOM, PIN_INTAKE_BOTTOM_STOP, PIN_PHOTO_1, PIN_PHOTO_2, 
PIN_PHOTO_3, PIN_ELEVATOR_BOTTOM_HEIGHT, PIN_ELEVATOR_TOP_HEIGHT, 
PIN_MOTOR_RUNNING};

void setup() 
{
    lcd1.begin(16, 2);
    actualLcd.begin(16, 2);
    
    Serial.begin(9600);
    Serial.println("Waiting for Commands");
    lcd1.print("Hello World");
    actualLcd.print("Hello World!");

    for(int i = 0; i < sizeof(myPins) / sizeof(int); i++)
    {
      pinMode(myPins[i], OUTPUT);
    }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    String inputString = Serial.readStringUntil('\n');

    //0 indicates individual LED data
    if(inputString.charAt(0) == '0')
    {
      int charAtIndex = 1;
      int arrayIndex = 0;
      while(charAtIndex < inputString.length())
      {
        char c = inputString.charAt(charAtIndex);
        if(c == ':')
        {
          arrayIndex++;
        }
        else if(c == '0')
        {
          digitalWrite(myPins[arrayIndex], LOW);
        }
        else if(c == '1')
        {
          digitalWrite(myPins[arrayIndex], HIGH);
        }
        charAtIndex++;
      }
    }
    //1 indicates autoAlign LED strip data
    else if(inputString.charAt(0) == '1')
    {
      bool autoAligning = inputString.charAt(1) == '1';
      bool allSystemsGo = inputString.charAt(2) == '1';
      if(allSystemsGo)
        ledBlink();
      else if(autoAligning)
        ledSolid();
    }
    //2 indicates hood angle data
    else if (inputString.charAt(0) == '2')
    {
      String angle = inputString.substring(0,inputString.indexOf("\n"));
      writeHoodAngleData(angle);
    }
    //3 indicates shot distance at angle data
    else if (inputString.charAt(0) == '3')
    {
      String shot = inputString.substring(0,inputString.indexOf("\n"));
      writeShotDistanceData(shot);
    }
    //4 indicates shooter speed data
    else if (inputString.charAt(0) == '4')
    {
      String shooter_speed = inputString.substring(0,inputString.indexOf("\n"));
      writeActualShooterData(shooter_speed);
    }
    //5 indicates goal shooter speed data
    else if (inputString.charAt(0) == '5')
    {
      String goal_shooter_speed = inputString.substring(0,inputString.indexOf("\n"));
      writeGoalShooterData(goal_shooter_speed);
    }
  }
}

bool ledState = true;
int msgState = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 250;           // interval at which to blink (milliseconds)

void writeHoodAngleData(String angle) {
  lcd1.setCursor(0, 0);
  lcd1.println(angle);
}

void writeShotDistanceData(String distance) {
  lcd1.setCursor(0, 1);
  lcd1.println(distance);
}

void writeActualShooterData(String shooter_speed) {
  lcd2.setCursor(0, 0);
  lcd2.println(shooter_speed);
}

void writeGoalShooterData(String goal_shooter_speed) {
  lcd2.setCursor(0, 1);
  lcd2.println(goal_shooter_speed);

}

void ledBlink()
{
  if(msgState != 1)
  {
    Serial.println("LED BLINK");
    msgState = 1;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState) 
    {
      for(int i = 0; i < LED_COUNT; i++)
      {
        ledStrip.setPixelColor(i, 0x00FF00);
      }
    } 
    else 
    {
      for(int i = 0; i < LED_COUNT; i++)
      {
        ledStrip.setPixelColor(i, 0x000000);
      }
    }

    ledState = !ledState;
  }
}

void ledSolid()
{
  if(msgState != 2)
  {
    Serial.println("LED SOLID");
    msgState = 2;
  }

  for(int i = 0; i < LED_COUNT; i++)
  {
    ledStrip.setPixelColor(i, 0x00FF00);
  }
}

