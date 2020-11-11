#ifndef TRACKERUI_H
#define TRACKERUI_H

#include "Arduino.h"
#include "heltec.h"
#include "oled/OLEDDisplayUi.h"

class TrackerUI
{
private:
    OLEDDisplayUi ui;
    int frameCount = 1;

public:
    TrackerUI(/* args */);
    ~TrackerUI();
};

TrackerUI::TrackerUI(/* args */) : ui(Heltec.display)
{
}

TrackerUI::~TrackerUI()
{
}

#endif // !TRACKERUI_H