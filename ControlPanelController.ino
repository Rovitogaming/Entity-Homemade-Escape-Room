//Script by Dylan Rovito
//Made for "Entity" Escape Room
//CONTROL PANEL CONTROLLER SCRIPT FOR ARDUINO MEGA
//Controls Button and Keypad Puzzle

#include <Keypad.h>
#include <FastLED.h>
#define LED_PIN     10 //PIN 10
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

const int ButtonPin1 = 24;
const int ButtonPin2 = 26;
const int WifiIn = 30;
int Button1State = 0;
int Button2State = 0;
int WifiInState = 0;
bool Button2Ready = true;
bool ButtonWon = false;
bool ButtonStarted = false;
bool StopCursor = true;

const int WifiData = 39;
/////////MAIN VARIABLES/////////////
bool KPUnlocked = false;
int TotalKeyPress = 0;
int TargetLED = 1;
int CursorLED = 1;
int PCursorLED = 0;
int PuzzlesLeft = 6; //# - 1 = Puzzles Left
////////////////////////////////////

//Which Leds are used?
  int Led1 = 1; 
  int Led2 = 2;
  int Led3 = 3;
  int Led4 = 4;
  int Led5 = 5;
  int Led6 = 6;
  int Led7 = 7;
  bool DeviceEnabled = true; ///////////////////////////////CHANGE FOR FINAL VERSION!!
//////////////////////////////////
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const int BuzzerTone = 700; //Tone of buzzer in frequency 
const int Buzzer = 9; //Buzzer Pin (SHOULD BE PWM)
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
int keyStep = 0;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8}; //connect to the column pinouts of the keypad
//////////////////////
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  delay(3000); //Power Up Safety Delay
 FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
   pinMode(2,OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(15, OUTPUT);
   pinMode(31, OUTPUT);
   pinMode(WifiData, OUTPUT);
   pinMode(ButtonPin1, INPUT);
   pinMode(ButtonPin2, INPUT);
   pinMode(WifiIn, INPUT);
   digitalWrite(ButtonPin1, HIGH);
   digitalWrite(ButtonPin2, HIGH);
   Serial.begin(9600);
   Serial.println("CONTROL PANEL CONTROLLER V5.8");
}

void RandomLED() {
  if (PuzzlesLeft > 0) {
    StopCursor = false;
  leds[Led1] = CRGB::Blue;
  leds[Led2] = CRGB::Blue;
  leds[Led3] = CRGB::Blue;
  leds[Led4] = CRGB::Blue;
  leds[Led5] = CRGB::Blue;
  leds[Led6] = CRGB::Blue;
  leds[Led7] = CRGB::Blue;
  TargetLED = random(2,8);
  leds[TargetLED] = CRGB::Red;
  FastLED.show();
  }
}


void loop() {

Button1State = digitalRead(ButtonPin1);
Button2State = digitalRead(ButtonPin2);
WifiInState = digitalRead(WifiIn);


//GENERATE PUZZLE BUTTON
if (Button1State == HIGH) {
  if(ButtonStarted == false) {
  RandomLED();
  CursorLED = 1;
  ButtonStarted = true;
  
}
}

if (StopCursor == false) {
  delay(150);
  if (CursorLED == 7) {
    CursorLED = 1;
    PCursorLED = 0;
    leds[Led7] = CRGB::Blue;
  }
CursorLED = CursorLED +1;
PCursorLED = PCursorLED +1;
leds[TargetLED] = CRGB::Red;
leds[PCursorLED] = CRGB::Blue;
leds[CursorLED] = CRGB::Green;
FastLED.show();
}

//SUBMIT BUTTON
if(Button2State == HIGH) {
  if (Button2Ready == true) {
    if (ButtonWon == false) {
      if (ButtonStarted == true) {
  StopCursor = true;
  Button2Ready = false;
  delay(1000);
  if (CursorLED == TargetLED) {
    PuzzlesLeft = PuzzlesLeft -1; //CORRECT TARGET
    tone(Buzzer, 700, 700);
   leds[Led1] = CRGB::Green;
  leds[Led2] = CRGB::Green;
  leds[Led3] = CRGB::Green;
  leds[Led4] = CRGB::Green;
  leds[Led5] = CRGB::Green;
  leds[Led6] = CRGB::Green;
  leds[Led7] = CRGB::Green;
  FastLED.show();
  delay(700);
  RandomLED();
  CursorLED = 1;
  PCursorLED = 0;
  ButtonStarted = true;
  }
  else {
    //PuzzlesLeft = 4; //SET TO TRUE FOR FINAL VERSION MAYBE?!?!
    tone(Buzzer, 500, 700);
  leds[Led1] = CRGB::Red;
  leds[Led2] = CRGB::Red;
  leds[Led3] = CRGB::Red;
  leds[Led4] = CRGB::Red;
  leds[Led5] = CRGB::Red;
  leds[Led6] = CRGB::Red;
  leds[Led7] = CRGB::Red;
  FastLED.show();
  delay(700);
  RandomLED();
  CursorLED = 1;
  PCursorLED = 0;
  ButtonStarted = true;
  }
}
}
}
}

if(Button2State == LOW) {
  Button2Ready = true;
}

if (PuzzlesLeft == 1){
  if (ButtonWon == false) {
  tone(Buzzer, 1000, 100);
  leds[Led1] = CRGB::Green;
  leds[Led2] = CRGB::Green;
  leds[Led3] = CRGB::Green;
  leds[Led4] = CRGB::Green;
  leds[Led5] = CRGB::Green;
  leds[Led6] = CRGB::Green;
  leds[Led7] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[Led1] = CRGB::Black;
  leds[Led2] = CRGB::Black;
  leds[Led3] = CRGB::Black;
  leds[Led4] = CRGB::Black;
  leds[Led5] = CRGB::Black;
  leds[Led6] = CRGB::Black;
  leds[Led7] = CRGB::Black;
  FastLED.show();
  delay(100);
  tone(Buzzer, 1000, 100);
  leds[Led1] = CRGB::Green;
  leds[Led2] = CRGB::Green;
  leds[Led3] = CRGB::Green;
  leds[Led4] = CRGB::Green;
  leds[Led5] = CRGB::Green;
  leds[Led6] = CRGB::Green;
  leds[Led7] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[Led1] = CRGB::Black;
  leds[Led2] = CRGB::Black;
  leds[Led3] = CRGB::Black;
  leds[Led4] = CRGB::Black;
  leds[Led5] = CRGB::Black;
  leds[Led6] = CRGB::Black;
  leds[Led7] = CRGB::Black;
  FastLED.show();
  delay(100);
  tone(Buzzer, 1000, 100);
  leds[Led1] = CRGB::Green;
  leds[Led2] = CRGB::Green;
  leds[Led3] = CRGB::Green;
  leds[Led4] = CRGB::Green;
  leds[Led5] = CRGB::Green;
  leds[Led6] = CRGB::Green;
  leds[Led7] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[Led1] = CRGB::Black;
  leds[Led2] = CRGB::Black;
  leds[Led3] = CRGB::Black;
  leds[Led4] = CRGB::Black;
  leds[Led5] = CRGB::Black;
  leds[Led6] = CRGB::Black;
  leds[Led7] = CRGB::Black;
  FastLED.show();
  ButtonWon = true;
  KPUnlocked = true;
Button2Ready = false;
ButtonStarted = true;
StopCursor = true;
  }
}
////////////////////////////////////////////////
//////////////KEYPAD VERIFICATION/////////////////
if (KPUnlocked == true) {

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
  
      if (key =='9') {
        if (keyStep == 1) {
        Serial.println("Key 9 is pressed");
        keyStep = 2;
      }
      }

     if (key =='5') {
        if (keyStep == 2) {
        Serial.println("Key 5 is pressed");
        keyStep = 3;
      }
      }

     if (key =='0') {
        if (keyStep == 3) {
        Serial.println("Key 0 is pressed");
        keyStep = 4;
      }
      }
if (key =='#') {
  if (keyStep == 4) {
  Serial.println("Correct Passcode");
  tone(Buzzer, 1000, 1000);
  leds[Led4] = CRGB::Black;
    leds[Led3] = CRGB::Black;
    leds[Led2] = CRGB::Black;
    leds[Led1] = CRGB::Black;
    FastLED.show();
  digitalWrite(WifiData, HIGH);
  delay(2000);
  digitalWrite(WifiData, LOW);;
  KPUnlocked = false;
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

int AlarmLED = 10;
while (WifiInState == HIGH) {
  leds[Led1] = CRGB::Black;
  leds[Led2] = CRGB::Black;
  leds[Led3] = CRGB::Black;
  leds[Led4] = CRGB::Black;
  leds[Led5] = CRGB::Black;
  leds[Led6] = CRGB::Black;
  leds[Led7] = CRGB::Black;
Button2Ready = false;
ButtonStarted = true;
StopCursor = true;
ButtonWon = true;
  FastLED.show();
  leds[AlarmLED] = CRGB::Red;
  FastLED.show();
  delay(100);
  leds[AlarmLED] = CRGB::Black;
  FastLED.show();
  delay(100);
}
   }
  
