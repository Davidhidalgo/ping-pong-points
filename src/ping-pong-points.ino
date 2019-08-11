#include <Arduino.h>
#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include "BitmapGraphics.h"

//#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#include <AceButton.h>
using namespace ace_button;

const char *name = "FreeSansBold18pt7b";
const GFXfont *fontResult = &FreeSansBold18pt7b;

GxIO_Class io(SPI, SS, 22, 21);
GxEPD_Class display(io, 16, 4);

const uint8_t BUTTON1_PIN = 12;
const uint8_t BUTTON2_PIN = 12;

AceButton buttonPlayer1(BUTTON1_PIN);

void handleEvent(AceButton *, uint8_t, uint8_t);

char DASHES[2] = {'-', '-'};

const int matchPoints = 9;
bool matchStarted = false;

int player1Score = 0;
int player2Score = 0;
int playerServing = -1;

void setup()
{
    Serial.begin(9600);
    // display.init();

    // //display.drawExampleBitmap(gImage_splash, 0, 0, 200, 200, GxEPD_BLACK);
    // display.fillScreen(GxEPD_WHITE);
    // display.update();

    // String texto = "10";

    // display.setRotation(90);

    // printGrid();
    // display.update();
    // setResultFont();

    // display.drawExampleBitmap(gImage_gui_f, sizeof(gImage_gui_f), GxEPD::bm_default | GxEPD::bm_partial_update);

    // // display.drawExampleBitmap(gImage_gui_f, 0, 0, 200, 200, GxEPD_BLACK);
    // // delay(2000);

    // display.drawExampleBitmap(gImage_gui_f, sizeof(gImage_gui_f), GxEPD::bm_default | GxEPD::bm_partial_update);
    pinMode(BUTTON1_PIN, INPUT);
    buttonPlayer1.init(BUTTON1_PIN);

    pinMode(LED_BUILTIN, OUTPUT);

    buttonPlayer1.setEventHandler(handleEvent);
}

void loop()
{
    // buttonPlayer1.check();
    // buttonPlayer2.check();

    // displayScore();
    // printPlayer1();
    // delay(500);
    // printPlayer2();
    // delay(2000);
    buttonPlayer1.check();
    // Serial.println("  PACHA   ");
}

void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t /* buttonState */)
{
    switch (eventType)
    {
    case AceButton::kEventPressed:
        Serial.println("Pressed!");
        digitalWrite(LED_BUILTIN, HIGH);
        break;
    case AceButton::kEventReleased:
        Serial.println("Released!");
        digitalWrite(LED_BUILTIN, LOW);
        break;
    }
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
    player1Score += 1;
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
    player2Score += 1;
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