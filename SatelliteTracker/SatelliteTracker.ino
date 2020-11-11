#include "Arduino.h"
#include "heltec.h"
#include "Tracker.h"
#include "Connection.h"
#include "Display.h"

#include <TimeLib.h>

Tracker issTracker;
Observer observer;
Connection conn;

time_t getTime()
{
    return conn.getNtpTime();
}

void outPutCallback(String str)
{
    Serial.print(str);
}

void setup()
{
    Display::begin();
    Display::clear();

    Serial.begin(115200);

    conn.setOutputCallback(Display::drawString);

    conn.begin();

    setSyncProvider(getTime);
    setSyncInterval(300);
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
    if (timeStatus() != timeNotSet)
    {
        if (now() != prevDisplay)
        { 
            //update the display only if time has changed
            Display::clear();

            prevDisplay = now();
            Display::digitalClockDisplay();

            Display::drawSatName(String(issTracker.tleName));

            issTracker.settime(year(), month(), day(), hour(), minute(), second());

            double dSatEL, dSatAZ;
            issTracker.elaz(observer, dSatEL, dSatAZ);

            Display::drawAzEl(dSatAZ, dSatEL);

            double dSatLAT, dSatLON;
            issTracker.latlon(dSatLAT, dSatLON);

            Display::drawLatLon(dSatLAT, dSatLON);
            
            Display::display();
        }
    }
}

