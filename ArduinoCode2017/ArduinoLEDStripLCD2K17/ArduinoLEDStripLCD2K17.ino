#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
//#include "WS2812_Definitions.h"

#define PIN 46
#define LED_COUNT 45
#define START_LED 's'
#define END_LED 'e'
#define TOT_LEN 4 + (3*LED_COUNT)


#define LCD_X 16
#define LCD_Y 2
#define LCD_RS 53
#define LCD_E 52
#define LCD_DB4 51
#define LCD_DB5 50
#define LCD_DB6 49
#define LCD_DB7 48

//START, #LEDs, start pos(LED # starting from 0 -> LED_COUNT-1, ... END
//START_LED means begin parsing for LED strip display
//END_LED means display states array
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

int totLen = TOT_LEN;
char states[LED_COUNT][3];//2d array LED_COUNT x 3
char message[TOT_LEN];




void setup() {  
  Serial.begin(9600);
  Serial.println("Hello World!");
  strip.begin();
  lcd.begin(LCD_X, LCD_Y);
  
  for(int i = 0; i < LED_COUNT; i++) {//clear all states
    states[i][0] = 0;
    states[i][1] = 0;
    states[i][2] = 0;
  }

  clearLEDs();
  strip.show();
}

int n = 0, i = 0;
void loop() {
  //lcd.print("Hello World!");
  //i = testLEDs(i);
  //parseInfo();

  //countDown(15);
    countDown2(15);
  
  //for(int i = 0; i < LED_COUNT; i++) {
    //states[i][0] = 0xFF;
  //}
  //drawStates();

  
}



//////////////////fubctions




void clearLEDs() {
  for (int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, 0);
    states[i][0] = 0;
    states[i][1] = 0;
    states[i][2] = 0;
  }
}

void setLED(int n, char r, char g, char b) {//ex:(5, 0xFF, 0xE1, 0xF3)
  strip.setPixelColor(n, r, g, b);
  strip.show();//if lag, put this after a large number of setPixelColor() 's
}

int testLEDs(int i) {
  //clearLEDs();
  for(int j = 0; j < LED_COUNT; j++) {
    setLED(j, (i+j)%0xFF, (i+0xAA+j)%0xFF, (i+0x55+j)%0xFF);
  }
  i++;
  i = i%0xFF;
  return i;
}

void countDown(int seconds) {
  for(int j = seconds; j>0; j--) {
    delay(1000);
    if(j<seconds && j>(seconds/3)*2) {
      for(int i=0; i<LED_COUNT; i++) {
        setLED(i, 0x00, 0xFF, 0x00);
      }
    }
     if(j<(seconds/3)*2 && j> seconds/3) {
       for(int i=0; i<LED_COUNT; i++) {
         setLED(i, 0x00, 0x00, 0xFF);
       }
     }
     if(j<seconds/3) {
       for(int i=0; i<LED_COUNT; i++) {
         setLED(i, 0xFF, 0x00, 0x00);
       }
     }
  }
}

void countDown2(int seconds) {
  for(int LED = LED_COUNT; LED >= 0; LED--) {
    delay((seconds/LED_COUNT)*1000);
    states[LED][0] = 0xFF;
    states[LED][1] = 0xFF;
    states[LED][2] = 0xFF;
    drawStates();
  }
}

void drawStates() {
  for(int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i+1, states[i][0], states[i][1], states[i][2]);//i+1 because I think hat is where the first LED in indexed
  }
  strip.show();
}

char readSerial() {
  int a = Serial.read();
  while(1) {
    if(a == -1) {
      a = Serial.read();
    }else {
      return a;
    }
  }
}

void parseInfo() {
  int val = Serial.read();
  int numLeds;
  int startPos;
  int lcdX = 0;
  int lcdY = 0;
  int lcdStrLen = 0;
  int temp = 0;
  if(val != -1) {//recieved a value
    message[0] = val;//start?
    if(val == START_LED) {//for the leds
      Serial.print("\nlooking for data:");
      Serial.print("\n\tnumLEDs: ");
      numLeds = readSerial();//transmit the char val of it
      Serial.print(numLeds);
      Serial.print(", satartPos: ");
      startPos = readSerial() - 1;//
      Serial.print(startPos);
      Serial.print("\nlooking for colors");
      for(int i = startPos; i < numLeds + startPos; i++) {
        states[i][0] = readSerial();
        states[i][1] = readSerial();
        states[i][2] = readSerial();
        Serial.print("\tr, g, b: ");
        Serial.print(states[i]);
      }
      Serial.print("\ndrawing states");
      drawStates();
    }else if(val == 'L') {
      val = readSerial();
      if(val == 'C') {
        val = readSerial();
        if(val == 'D') {
          //begin code of 'LCD' found
          Serial.print("\nlooking for LCD data:\n\tx: ");
          lcdX = readSerial();
          Serial.print(lcdX);
          Serial.print("\n\ty: ");
          lcdY = readSerial();
          Serial.print(lcdY);
          lcd.setCursor(lcdX, lcdY);
          Serial.print("\n\t strLen: ");
          lcdStrLen = readSerial();
          Serial.print("\n\tstr: ");
          for(int i = 0; i < lcdStrLen; i++) {
            temp = readSerial();
            Serial.print(temp);
            lcd.print(temp);
          }
        }
      }
    }
  }
}

