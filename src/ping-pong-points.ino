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

void handleEvent(AceButton *, uint8_t, uint8_t);


const uint8_t NUM_PLAYERS = 2;
const int MATCH_POINTS = 11;

const uint8_t BUTTON_PLAYER1_PIN = 15;
const uint8_t BUTTON_PLAYER2_PIN = 2;

bool matchStarted = false;

struct Player {
    const uint8_t buttonPin;
    uint8_t score;
    bool serving;
};

Player PLAYERS[NUM_PLAYERS] = {
    {BUTTON_PLAYER1_PIN, 0, 0},
    {BUTTON_PLAYER2_PIN, 0, 0}
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

    printPlayersScore();
}

void printGrid()
{
    display.fillRect(0, 0, 200, 100, GxEPD_BLACK);
    display.fillRect(4, 4, 192, 92, GxEPD_WHITE);
    display.fillRect(98, 0, 4, 100, GxEPD_BLACK);
}


void setResultFont()
{
    display.setFont(fontResult);
    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(2);
}

void printPlayerScore(uint8_t player_id)
{
    struct Box {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        uint8_t h;
        
    };
    Box Boxes[NUM_PLAYERS] {
        {5, 5, 92, 90},
        {103, 5, 92, 90},
    };

    Box box = Boxes[player_id];
    Player player = PLAYERS[player_id];

    int xPosition = 7;
    if (player.score < 10)
    {
        xPosition = 25;
    }

    display.fillRect(box.x, box.y, box.w, box.h, GxEPD_WHITE);
    display.setCursor(box.x + xPosition, 72);
    display.print(player.score);
    display.updateWindow(box.x, box.y, box.w, box.h, true);
}

void printPlayersScore() {
    for (uint8_t i=0; i< NUM_PLAYERS; i++) {
        printPlayerScore(i);
    }
}

void handleEvent(AceButton * button, uint8_t eventType, uint8_t buttonState)
{
    uint8_t id = button->getId();

    switch (eventType) {
        case AceButton::kEventReleased:
            if (gameFinished()) {
                PLAYERS[0].score = 0;
                PLAYERS[1].score = 0;
                printPlayersScore();
                matchStarted = false;
                return;
            } else {
                matchStarted = true;
            }

            if (matchStarted) {
                PLAYERS[id].score += 1;
                printPlayerScore(id);
            }
            break;
    }

}

bool gameFinished() {
    if (PLAYERS[0].score >= MATCH_POINTS && (PLAYERS[0].score - PLAYERS[1].score) >=2) {
        Serial.println("Player 1 WINS");
        return true;
    } else if (PLAYERS[1].score >= MATCH_POINTS && (PLAYERS[1].score - PLAYERS[0].score >=2)) {
        Serial.println("Player 2 WINS");
        return true;
    };
    return false;
}
