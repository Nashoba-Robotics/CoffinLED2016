#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
//#include "WS2812_Definitions.h"

#define PIN 8
#define LED_COUNT 45
#define START_LED 's'
#define END_LED 'e'
#define TOT_LEN 4 + (3*LED_COUNT)


#define LCD_X 16
#define LCD_Y 2
#define LCD_RS 2
#define LCD_E 3
#define LCD_DB4 4
#define LCD_DB5 5
#define LCD_DB6 6
#define LCD_DB7 7

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

int n = 0;
void loop() {
  parseInfo();
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

void setLED(int n, char r, char g, char b) {
  strip.setPixelColor(n, r, g, b);
  strip.show();//if lag, put this after a large number of setPixelColor() 's
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

