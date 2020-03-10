//Script by Dylan Rovito
//Made for "Entity" Escape Room
//CONTROL PANEL CONTROLLER SCRIPT FOR ARDUINO MEGA
//Controls Button and Keypad Puzzle

#include <Keypad.h>
#include <FastLED.h>
#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

/////////MAIN VARIABLES/////////////
bool KPUnlocked = false;
int TotalKeyPress = 0;
bool ButtonsSolved = false;
bool ButtonsDisabled = false;
int BSolveProgress = 0;
////////////////////////////////////

//Pin number of buttons should be in proper order eg. b1, b2, b3, b4
const int Button1 = 22; 
const int Button2 = 24;
const int Button3 = 26;
const int Button4 = 28;
///////////////////////////

int Button1State = 1;
int Button2State = 1;
int Button3State = 1;
int Button4State = 1;

//Which Leds are used?
  int Led1 = 1; 
  int Led2 = 2;
  int Led3 = 3;
  int Led4 = 4;
  bool DeviceEnabled = false;
//////////////////////////////////
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const int BuzzerTone = 700; //Tone of buzzer in frequency 
const int Buzzer = 5; //Buzzer Pin (SHOULD BE PWM)
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
int keyStep = 0;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//KEY PAD PINS
//ORIGINAL//
//byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {6,7,8}; //connect to the column pinouts of the keypad

//NEW//
byte rowPins[ROWS] = {52, 50, 48, 46}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {44,42,40}; //connect to the column pinouts of the keypad
//////////////////////
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  delay(3000); //Power Up Safety Delay
 FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
   pinMode(Button1, INPUT);
   pinMode(Button2, INPUT);
   pinMode(Button3, INPUT);
   pinMode(Button4, INPUT);
   pinMode(2,OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(15, OUTPUT);

}

void ButtonFail() {
  Serial.print("ButtonFail");
  ButtonsDisabled = true;
  KPUnlocked = false;
  leds[Led1] = CRGB::Red; 
        FastLED.show();
        leds[Led2] = CRGB::Red; 
        FastLED.show();
        leds[Led3] = CRGB::Red; 
        FastLED.show();
        leds[Led4] = CRGB::Red; 
        FastLED.show();
        delay(500);
        leds[Led1] = CRGB::Black; 
        FastLED.show();
        leds[Led2] = CRGB::Black; 
        FastLED.show();
        leds[Led3] = CRGB::Black; 
        FastLED.show();
        leds[Led4] = CRGB::Black; 
        FastLED.show();
        delay(500);
        leds[Led1] = CRGB::Red; 
        FastLED.show();
        leds[Led2] = CRGB::Red; 
        FastLED.show();
        leds[Led3] = CRGB::Red; 
        FastLED.show();
        leds[Led4] = CRGB::Red; 
        FastLED.show();
        delay(500);
        leds[Led1] = CRGB::Black; 
        FastLED.show();
        leds[Led2] = CRGB::Black; 
        FastLED.show();
        leds[Led3] = CRGB::Black; 
        FastLED.show();
        leds[Led4] = CRGB::Black; 
        FastLED.show();
        ButtonsDisabled = false;
}


void loop() {
   if (DeviceEnabled == true) {
////////////////////////////////////////////////
Button1State = digitalRead(Button1);
  Button2State = digitalRead(Button2);
  Button3State = digitalRead(Button3);
  Button4State = digitalRead(Button4);

  ////////////////////////////////////////////////////
  if (Button3State == HIGH) {
    if (ButtonsDisabled == false) {
      if (BSolveProgress == 0) {
      BSolveProgress = 1;
      leds[Led1] = CRGB::Green; 
        FastLED.show();
    }
   else ButtonFail();
    }
  }
///////////////////////////////////////////////////////
if (Button1State == HIGH) {
    if (ButtonsDisabled == false) {
      if (BSolveProgress == 1) {
      BSolveProgress = 2;
      leds[Led2] = CRGB::Green; 
        FastLED.show();
    }
   else ButtonFail();
    }
  }
////////////////////////////////////////////////////
if (Button2State == HIGH) {
    if (ButtonsDisabled == false) {
      if (BSolveProgress == 2) {
      BSolveProgress = 3;
      leds[Led3] = CRGB::Green; 
        FastLED.show();
    }
   else ButtonFail();
    }
  }
//////////////////////////////////////////////////
if (Button4State == HIGH) {
    if (ButtonsDisabled == false) {
      if (BSolveProgress == 3) {
      BSolveProgress = 4;
      leds[Led4] = CRGB::Green; 
        FastLED.show();
        KPUnlocked = true;
    }
   else ButtonFail();
    }
  }

  
//////////////KEYPAD VERIFICATION/////////////////
  char key = keypad.getKey(); 

    if (key) {
      tone(Buzzer, BuzzerTone, 100);
      TotalKeyPress = TotalKeyPress+1;
    }
   
    if (key =='4'){
    if (keyStep == 0) {
      Serial.println("Key 4 is pressed");
      keyStep = 1;
    }
    
      }
  
      if (key =='7') {
        if (keyStep == 1) {
        Serial.println("Key 7 is pressed");
        keyStep = 2;
      }
      }

     if (key =='2') {
        if (keyStep == 2) {
        Serial.println("Key 2 is pressed");
        keyStep = 3;
      }
      }

     if (key =='1') {
        if (keyStep == 3) {
        Serial.println("Key 1 is pressed");
        keyStep = 4;
      }
      }
if (key =='#') {
  if (keyStep == 4) {
  Serial.println("Correct Passcode");
  tone(Buzzer, 1000, 1000);
      /////////////////////////////////////////CORRECT KEYPAD CODE
} else {
Serial.println("Incorrect Passcode");
tone(Buzzer, 550, 100);
delay(200);
tone(Buzzer, 550, 100);
delay(200);
tone(Buzzer, 550, 100);
TotalKeyPress = 0;
keyStep = 0;
}
}
if (key =='*') {
  keyStep = 0;
  TotalKeyPress = 0;
  Serial.println("Cleared");
  tone(Buzzer, BuzzerTone, 1000);
}

if (TotalKeyPress == 6) {
  Serial.println("Incorrect Passcode");
  tone(Buzzer, 550, 100);
  delay(200);
  tone(Buzzer, 550, 100);
  delay(200);
  tone(Buzzer, 550, 100);
  keyStep = 0;
  TotalKeyPress = 0;
}
///////////////////////////////////////////////////////////////
 }
  
/////
  }
/////
