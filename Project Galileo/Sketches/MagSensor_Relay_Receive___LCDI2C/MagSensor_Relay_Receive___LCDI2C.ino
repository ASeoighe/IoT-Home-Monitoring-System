#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


String msg;
char t;
int i;

int led = 4;

String Startup = "Setup, Complete..Ready!";
String DoorMsg = "Door Has Been Opened";
/*************************************************/
int MagSens1 = 4;
int MagSens2 = 5;
int MagSens3 = 6;
/*************************************************/
int MagState = 0;


LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup()
{
  
  digitalWrite(led, HIGH);
  Serial.println("Relay OFF");

  /*************************************************/
  pinMode(MagSens3, INPUT);
  pinMode(led, OUTPUT);
  /*************************************************/
  Serial.begin(9600);
  Serial1.begin(9600);// the GPRS baud rate
  //Serial1.println("AT+CLCK=\"SC\",0,\"3306\"");
  
  for(i=0;i<11;i++){
  Serial1.print("AT+CMGD=");
  Serial1.println(i);
  Serial1.print("\r\nl");
  delay(1000);}
  
  
  digitalWrite(led, HIGH);
  delay(1000);
  msg = Startup;
  
  lcdSetup();
  lcd.print("Setup Complete!");
  lcd.clear();
  SendTextMessage();
  
}

void loop()
{
   MagState = digitalRead(MagSens3);

   if (MagState == LOW)
   {
      //msg = t;
      msg = DoorMsg;
      SendTextMessage();
   }
  

  //digitalWrite(led, HIGH);
  //Serial1.println("AT+CMGD=1");
  //Serial.println("Relay OFF");
  ReceiveTextMessage();
  delay(2000);


}


/*****************************************************/
void SendTextMessage()
{
  Serial1.println("AT+CMGF=1");  //Because we want to send the SMS in text mode
  delay(1000);
  Serial1.println("AT+CMGS=\"+353876373466\"");
  delay(1000);
  Serial1.println(msg);
  delay(1000);
  Serial1.write(0x1A);//the ASCII code of the ctrl+z
  delay(1000);
  Serial1.println();
}

/***************************************************************/
void ReceiveTextMessage()
{
  Serial1.println("AT+CMGF=1");  //Because we want to receive the SMS in text mode
  delay(1000);

  Serial1.println("AT+CPMS=\"SM\"");  // read first SMS
  //Serial1.println("AT+CMGD=0,4");
  delay(1000);
  //Serial1.println("AT+CMGL=\"ALL\""); // show message
  Serial1.println("AT+CMGR=1");
  delay(500);
  /*for(i=0;i<11;i++){
  Serial1.print("AT+CMGD=");
  Serial1.println(i);
  Serial1.print("\r\nl");
  delay(1000);}*/


  while (Serial1.available() > 0) {
    t = Serial1.read();
    lcd.setCursor(2,1);
    lcd.print("Message Received");
    lcd.clear();
    //Serial.println("Message Received : ");
    Serial.print(t);
    if (t == 'o') {
      t = Serial1.read();
      if (t == 'n') {
        digitalWrite(led, LOW);
        Serial.println("Relay On");
        lcd.print("Light is On");
        lcd.clear();
        Serial1.println("AT+CMGD=1");
        delay(500);
      }
      else if (t != 'n')
     {
      if( t == 'f')
      {
        t = Serial1.read();
        if (t = 'f')
        {
          digitalWrite(led, HIGH);
          Serial.println("Relay Off");
          lcd.print("Light is Off");
          lcd.clear();
          Serial1.println("AT+CMGD=1");
          delay(500);
        }
        else {
          break;
        }
      }
      else if (t != 'n' || t == 'f') {
        break;
      }

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
