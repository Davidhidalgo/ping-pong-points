// Connection diagram
//
// # Button
//
//    ESP32  -->   BUTTON #1   
//    5V           Pin #1 
//    G15          Pin #2
//    
//    ESP32  -->   BUTTON #2   
//    5V           Pin #1 
//    G2           Pin #2
//
//    Note: There is no need to use a resistance nor GND connecion, since ESP32 already has it.
//
// # e-Paper modele 
//
//
//     ESP32  -->   ePaper
//     G4           BUSY
//     G21          RST
//     G22          DC
//     G5           CS
//     G18          CLK
//     G23          DIN
//     GND          GND


#include <Arduino.h>
#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include "BitmapGraphics.h"

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#include <AceButton.h>

const char *name = "FreeSansBold18pt7b";
const GFXfont *fontResult = &FreeSansBold18pt7b;

GxIO_Class io(SPI, SS, 22, 21);
GxEPD_Class display(io, 16, 4);

using namespace ace_button;

//const int BUTTON1_PIN = 15;
//const int BUTTON2_PIN = 2;

void handleEvent(AceButton *, uint8_t, uint8_t);

char DASHES[2] = {'-', '-'};

const int matchPoints = 9;
bool matchStarted = false;

int player1Score = 0;
int player2Score = 0;
int playerServing = -1;

const uint8_t NUM_PLAYERS = 2;

struct Player {
    const uint8_t buttonPin;
    const uint8_t score;
};

Player PLAYERS[NUM_PLAYERS] = {
    {15, 0},
    {2, 0}
};

AceButton buttons[NUM_PLAYERS];

void setup()
{
    Serial.begin(115200);
    
    setupDisplay();

    for (uint8_t i=0; i < NUM_PLAYERS; i++) {
        pinMode(PLAYERS[i].buttonPin, INPUT_PULLDOWN);
        buttons[i].init(PLAYERS[i].buttonPin, HIGH, i);
        buttons[i].setEventHandler(handleEvent);
    }
}

void loop()
{ 
    for (uint8_t i=0; i < NUM_PLAYERS; i ++) {
        buttons[i].check();
    }
}

void setupDisplay() {
    display.init();
 
    display.drawExampleBitmap(gImage_splash, 0, 0, 200, 200, GxEPD_BLACK);
    display.fillScreen(GxEPD_WHITE);
    display.update();
    
    display.setRotation(90);

    printGrid();
    display.update();
    setResultFont();
}

void setResultFont()
{
    display.setFont(fontResult);
    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(2);
}

void printPlayer1()
{
    int box_x = 5;
    int box_y = 5;
    int box_w = 92;
    int box_h = 90;

    int xPosition = 7;
    if (player1Score < 10)
    {
        xPosition = 25;
    }

    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    //display.drawRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.setCursor(box_x + xPosition, 72);
    display.print(player1Score);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
    
    if (player1Score >= 11) {
        //display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        //display.drawRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
        display.fillScreen(GxEPD_WHITE);
        display.update();
        display.setCursor(box_x + xPosition, 72);
        display.setTextSize(0.25);       
        display.print("todo el rato trap");
        display.update();

       }
}

void printPlayer2()
{
    int box_x = 103;
    int box_y = 5;
    int box_w = 92;
    int box_h = 90;

    int xPosition = 7;
    if (player2Score < 10)
    {
        xPosition = 25;
    }

    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    //display.drawRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.setCursor(box_x + xPosition, 72);
    display.print(player2Score);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
}

void printGrid()
{
    display.fillRect(0, 0, 200, 100, GxEPD_BLACK);
    display.fillRect(4, 4, 192, 92, GxEPD_WHITE);
    display.fillRect(98, 0, 4, 100, GxEPD_BLACK);
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
    // String result = String(player1Score); // + String(player2Score);

    // if (!matchStarted)
    // {
    //     displayDashes();
    // }
    // else
    // {
    //     sevseg.setNumber(result.toInt()); //, playerServing);
    // }
    // sevseg.refreshDisplay();
}

void displayDashes()
{
    // sevseg.setChars(DASHES);
}

void handleEvent(AceButton * button, uint8_t eventType, uint8_t buttonState)
{
    uint8_t id = button->getId(); 

    switch (eventType)
    {
    case AceButton::kEventReleased:
        Serial.println(id);
        if (id==0) {
            player1Score += 1;
            printPlayer1();
        } else if (id==1) {
            player2Score += 1;
            printPlayer2();
        }
        break;
    }
}