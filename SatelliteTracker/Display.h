#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "heltec.h"

namespace Display
{
    void begin()
    {
        Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
        Heltec.display->setContrast(255);
        Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
        Heltec.display->setFont(ArialMT_Plain_10);
    }

    void clear()
    {
        Heltec.display->clear();
    }

    void display()
    {
        Heltec.display->display();
    }

    void drawSatName(String sat)
    {
        Heltec.display->drawString(0, 12, sat);
    }

    void drawString(String str)
    {
        Heltec.display->drawString(0, 0, str);
    }

    void drawAzEl(double dAZ, double dEL)
    {
        String str = "";
        str.concat("Az: ");
        str.concat(String(dAZ));
        str.concat(" ");
        str.concat("El: ");
        str.concat(String(dEL));
        Heltec.display->drawString(0, 24, str);
    }

    void drawLatLon(double dLat, double dLon)
    {
        String str = "";
        str.concat("Lat: ");
        str.concat(String(dLat));
        str.concat(" ");
        str.concat("Lon: ");
        str.concat(String(dLon));
        Heltec.display->drawString(0, 36, str);
    }

    void printDigits(int digits, String &str)
    {
        // utility for digital clock display: prints preceding colon and leading 0
        str.concat(":");
        if (digits < 10)
            str.concat('0');
        str.concat(String(digits));
    }

    void digitalClockDisplay()
    {

        // digital clock display of the time
        String str = "";

        //Serial.print(hour());
        str.concat(String(hour()));

        printDigits(minute(), str);
        printDigits(second(), str);

        Heltec.display->drawString(0, 0, str);
    }
} // namespace Display

#endif // !DISPLAY_H