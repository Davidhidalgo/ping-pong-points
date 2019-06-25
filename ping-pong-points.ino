#include "SevSeg.h"
#include <AceButton.h>
using namespace ace_button;

SevSeg sevseg;

const int BUTTON1_PIN = 10;
const int BUTTON1_PIN = 12;
AceButton buttonPlayer1;
AceButton buttonPlayer2;

void handleEvent(AceButton*, uint8_t, uint8_t);

char DASHES[2] = {'-', '-'};

const int matchPoints = 9;
bool matchStarted = false;
int player1Score = 0;
int player2Score = 0;
int playerServing = -1;

 void setup()
 {
    byte numDigits = 2;
    byte digitPins[] = {12, 13};
    byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
    bool resistorsOnSegments = true;
    bool updateWithDelays = false; 
    bool leadingZeros = true;
    byte hardwareConfig = COMMON_ANODE;
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
    sevseg.setBrightness(200);
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    //pinMode(button2Pin, INPUT_PULLUP);

    buttonPlayer1.init()
    buttonPlayer1.setEventHandler(handleEvent);

    Serial.begin(9600);
}

 void toggleServe()
 {
     bool changeServe = (player1Score + player2Score) % 2 == 0;
     if (changeServe)
     {
         if (playerServing == 1)
         {
             playerServing = 0;
         }
         else
         {
             playerServing = 1;
         }
     }
 }

 void resetMatch()
 {
     player1Score = 0;
     player2Score = 0;
     matchStarted = false;
 }

 void displayScore()
 {
     String result = String(player1Score); // + String(player2Score);
     
     if (!matchStarted) {
         displayDashes();
     } else {
        sevseg.setNumber(result.toInt());//, playerServing);
     }
     sevseg.refreshDisplay();   
 }

 void displayDashes()
 {
     sevseg.setChars(DASHES);
 }

void loop() 
{
    buttonPlayer1.check();
    buttonPlayer2.check();

    displayScore();
}

void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventReleased:
      if (!matchStarted) {
          matchStarted = true;
      } else {
        player1Score += 1;
      }
      break;
  }
}