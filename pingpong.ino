#include "SevSeg.h"
SevSeg sevseg;

const int button1Pin = 10;
const int button2Pin = 11;
int button1State = 0;
int button2State = 0;

char DASHES[2] = {'-', '-'};

unsigned long previousMillis = 0;
const long interval = 1000;

const int matchPoints = 9;
bool matchStarted = false;
int player1 = 0;
int player2 = 0;
int playerServing = -1;

void setup()
{
    byte numDigits = 2;
    byte digitPins[] = {12, 13};
    byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
    bool resistorsOnSegments = true;
    bool updateWithDelays = false; // Default 'false' is Recommended
    bool leadingZeros = true;      // Use 'true' if you'd like to keep the leading zeros

    byte hardwareConfig = COMMON_ANODE;
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
    sevseg.setBrightness(200);
    // initialize the pushbutton pin as an input:
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
    Serial.begin(9600);
}

void toggleServe()
{
    const bool changeServe = (player1 + player2) % 3 == 0;
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
    player1 = 0;
    player2 = 0;
    matchStarted = false;
}

void displayScore()
{
    String result = String(player1) + String(player2);
    sevseg.setNumber(result.toInt(), playerServing);
    sevseg.refreshDisplay();
}

void displayDashes()
{
    sevseg.setChars(DASHES);
    sevseg.refreshDisplay();
}

void loop()
{
    const int button1NewState = digitalRead(button1Pin);
    const int button2NewState = digitalRead(button2Pin);
    const bool button1Up = button1State == 1 && button1NewState == 0;
    const bool button2Up = button2State == 1 && button2NewState == 0;
    const bool somePlayerButtonUp = button1Up || button2Up;
    button1State = button1NewState;
    button2State = button2NewState;
    bool wonMatch = player1 == matchPoints || player2 == matchPoints;
    bool resetingMatch = (player1 > matchPoints || player2 > matchPoints) || (wonMatch && somePlayerButtonUp);

    if (resetingMatch)
    {
        resetMatch();
    }
    else
    {
        if (matchStarted)
        {
            if (button1Up)
            {
                player1 += 1;
                toggleServe();
            }
            if (button2Up)
            {
                player2 += 1;
                toggleServe();
            }

            displayScore();
        }
        else
        {
            displayDashes();
            if (somePlayerButtonUp)
            {
                matchStarted = true;
                if (button1Up)
                {
                    playerServing = 1;
                }
                if (button2Up)
                {
                    playerServing = 0;
                }
            }
        }
    }
}
