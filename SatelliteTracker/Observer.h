#ifndef OBSERVER_H
#define OBSERVER_H

#include <ArduinoP13.h>

struct Observer
{
    const char *pcMyName = "Observer"; // Observer name
    double dMyLAT = 57.702221;      // Latitude (Breitengrad): N -> +, S -> -
    double dMyLON = 11.93677;      // Longitude (Längengrad): E -> +, W -> -
    double dMyALT = 1.0;            // Altitude ASL (m)

    P13Observer toP13()
    {
        return P13Observer(pcMyName, dMyLAT, dMyLON, dMyALT);
    }
};

#endif // !OBSERVER_H