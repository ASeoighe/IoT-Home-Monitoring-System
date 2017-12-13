#ifndef INTERGRATE_ATTEMP_H
#define INTERGRATE_ATTEMP_H

#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

class Text
{
  public:
    void Send();
    void Receive();
  private:
    int Relay1 = 4;
    int Relay2 = 5;
    int Relay3 = 6;
    int Relay4 = 7;
    int Relay5 = 8;
    String msg;
    String Startup = "Setup, Complete..Ready!";
    String DoorMsg = "Door Has Been Opened";
    int i, n;

#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

};
class All
{
  public:
    void DeclareVariables();
    void Setup();
    void Includes();
    
  private:
    
};
class Web
{
  public:
    void Display(EthernetClient &server.available());
  private:
#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetClient client = server.available();

    byte mac[] = {0x98, 0x4F, 0xEE, 0x01, 0x81, 0xEA};
    IPAddress ip(192, 168, 137, 10);
    char* SensReadOut[] = {"OFF", "ON"};
    EthernetServer server(80);
    String HTTPRequest;
    // Store the LED's status (1: on; 0: off)
    int Relay1Status = 0;
    int Relay2Status = 0;
    int Relay3Status = 0;
    int Relay4Status = 0;
    int Relay1 = 4;
    int Relay2 = 5;
    int Relay3 = 6;
    int Relay4 = 7;
    int Relay5 = 8;    
    int i, n;


};

#endif // INTEGRATE_ATTEMP_H

