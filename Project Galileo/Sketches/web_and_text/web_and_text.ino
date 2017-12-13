#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>


String msg;
char t;
int i;
char* RelayNum;
int Relay;

int Relay1 = 4;
int Relay2 = 5;
int Relay3 = 6;

String Startup = "Setup, Complete..Ready!";
String DoorMsg = "Door Has Been Opened";
/*************************************************/
int MagSens1 = 4;
int MagSens2 = 5;
int MagSens3 = 6;
/*************************************************/
int MagState = 0;


LiquidCrystal_I2C lcd(0x3F, 20, 4);

byte mac[] = {0x98, 0x4F, 0xEE, 0x01, 0x81, 0xEA};
IPAddress ip(10, 3, 3, 149);
char* SensReadOut[] = {"OFF", "ON"};
EthernetServer server(80);

void setup()
{
  /*************************************************/
  pinMode(MagSens3, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  /*************************************************/
  Serial.begin(9600);
  while (!Serial) {};
  Serial1.begin(9600);// the GPRS baud rate
  Serial1.println("AT+CMGF=1");
  delay(2000);
  Serial1.println("AT+CMGD=1,4");
  delay(2000);
  msg = Startup;

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  lcdSetup();
  lcd.print("Setup Complete!");
  lcd.clear();
  SendTextMessage();

}

void loop()
{
  ReceiveTextMessage();
  delay(2000);
  if(i>0)
    digitalWrite(Relay1, LOW);
  else
    digitalWrite(Relay1, HIGH);
  WebCheck();

}


/*****************************************************/
void SendTextMessage()
{
  Serial1.println("AT+CMGF=1");  //Because we want to receive the SMS in text mode
  delay(2000);
  Serial1.println("AT+CMGS=\"+353876373466\"");
  delay(2000);
  Serial1.println(msg);
  delay(1000);
  Serial1.write(0x1A);//the ASCII code of the ctrl+z
  delay(1000);
  Serial1.println();
}

/***************************************************************/
int ReceiveTextMessage()
{
  //Serial1.println("AT+CMGF=1");  //Because we want to receive the SMS in text mode
  //delay(500);
  Serial1.println("AT+CPMS=\"SM\"");  // read first SMS
  delay(2000);
  // Serial1.println("AT+CMGD=1,4");
  // delay(200);
  Serial1.println("AT+CMGR=1");
  delay(2000);
 while (Serial1.available() > 0) {
    t = Serial1.read();
    Serial.print(t);
    if (t == 'o' || t == 'O') {
      t = Serial1.read();
      if (t == 'n') {
        i = 1;
        Serial1.println("AT+CMGD=1,4");
        delay(500);
        
      }
      else if (t != 'n' && t == 'f')
     {
        t = Serial1.read();
        if (t = 'f')
        {
          i = 0;
          Serial1.println("AT+CMGD=1,4");
          delay(500);
        }
        else {
          break;
        }
      }
      else if (t != 'n' || t == 'f') {
        break;
      }
Serial.println("\nEND OF MESSAGE");
    }
  }
}



void lcdSetup()
{
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print(Startup);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void WebCheck() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 1 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 4; analogChannel <= 6; analogChannel++) {
            int sensorReading = digitalRead(analogChannel);

            client.print("Relay");
            client.print(analogChannel - 3);
            client.print(" is ");

            if (sensorReading > 0)
            {
              client.print(SensReadOut[0]);
            }
            else
              client.print(SensReadOut[1]);

            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
