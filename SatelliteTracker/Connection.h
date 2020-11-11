#ifndef CONNECTION_H
#define CONNECTION_H

#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message

class Connection
{
private:
    void (*out)(String);

    const char *ssid = "kaesebrot"; //  your network SSID (name)
    const char *pass = "isteingutesbrot";      // your network password

    const char *ntpServerName = "us.pool.ntp.org";

    const int timeZone = 1;

    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

    WiFiUDP Udp;
    unsigned int localPort = 8888;  // local port to listen for UDP packets


public:
    Connection(/* args */);
    ~Connection();

    void begin();
    void setOutputCallback(void (*func)(String));

    time_t getNtpTime();
    void sendNTPpacket(IPAddress &address);
};

Connection::Connection(/* args */)
{
}

Connection::~Connection()
{
}

void Connection::begin()
{
    WiFi.begin(ssid, pass);

    out("Connecting to ");
    out(String(ssid));
    out("\n");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        out(".");
    }
    out("\n");
    out("Connected\n");

    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting UDP");
    Udp.begin(localPort);
    
}

void Connection::setOutputCallback(void (*func)(String))
{
    out = func;
}

time_t Connection::getNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() > 0)
        ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void Connection::sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

#endif // !CONNECTION_H