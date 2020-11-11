#ifndef TRACKER_H
#define TRACKER_H

#include "Arduino.h"
#include <ArduinoP13.h>

#include "Observer.h"

class Tracker
{
private:
    
    const char *tlel1 = "1 25544U 98067A   20306.70413515  .00002188  00000-0  47270-4 0  9990";
    const char *tlel2 = "2 25544  51.6475  28.0205 0001841  75.8763  82.3035 15.49367051253335";

    const int timeZone = 1;

    double dfreqRX = 145.800; // Nominal downlink frequency
    double dfreqTX = 437.800; // Nominal uplink frequency

    int iYear = 2020; // Set start year
    int iMonth = 11;  // Set start month
    int iDay = 1;    // Set start day
    int iHour = 18;   // Set start hour
    int iMinute = 17;  // Set start minute
    int iSecond = 37;  // Set start second

    P13DateTime MyTime;       // Set start time for the prediction
    P13Satellite MySAT;       // Create data from TLE

public:
    Tracker(/* args */);
    ~Tracker();

    void settime(int year, int month, int day, int h, int m, int s);
    void elaz(Observer &observer, double &dSatEL, double &dSatAZ);
    void latlon(double &dSatLAT, double &dSatLON);

    const char *tleName = "ISS (ZARYA)";
};

Tracker::Tracker(/* args */) : MySAT(tleName, tlel1, tlel2)
{
    MyTime.settime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
    Serial.printf("Year: %d, Month: %d", iYear, iMonth);
}

Tracker::~Tracker()
{
}

void Tracker::settime(int year, int month, int day, int h, int m, int s)
{
    MyTime.settime(year, month, day, h - timeZone, m, s);
}

void Tracker::elaz(Observer &observer, double &dSatEL, double &dSatAZ)
{
    MySAT.predict(MyTime);
    // double dSatLAT, dSatLON;
    // MySAT.latlon(dSatLAT, dSatLON);
    MySAT.elaz(observer.toP13(), dSatEL, dSatAZ);
}

void Tracker::latlon(double &dSatLAT, double &dSatLON)
{
    MySAT.predict(MyTime);
    MySAT.latlon(dSatLAT, dSatLON);
}

/*
double       dSatLAT  = 0;           // Satellite latitude
double       dSatLON  = 0;           // Satellite longitude
double       dSatAZ   = 0;           // Satellite azimuth
double       dSatEL   = 0;           // Satellite elevation

double       dSunLAT  = 0;           // Sun latitude
double       dSunLON  = 0;           // Sun longitude
double       dSunAZ   = 0;           // Sun azimuth
double       dSunEL   = 0;           // Sun elevation

int          ixQTH    = 0;           // Map pixel coordinate x of QTH
int          iyQTH    = 0;           // Map pixel coordinate y of QTH
int          ixSAT    = 0;           // Map pixel coordinate x of satellite
int          iySAT    = 0;           // Map pixel coordinate y of satellite
int          ixSUN    = 0;           // Map pixel coordinate x of sun
int          iySUN    = 0;           // Map pixel coordinate y of sun
*/
/*
char         acBuffer[20];            // Buffer for ASCII time

int          aiSatFP[32][2];          // Array for storing the satellite footprint map coordinates
int          aiSunFP[32][2];          // Array for storing the sunlight footprint map coordinates
*/

#endif // !TRACKER_H
