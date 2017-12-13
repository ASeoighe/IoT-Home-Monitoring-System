#include <String.h>

char data[256];
char t;
char c;
char p = 0;
String msg;
int led = 5;
String Startup = "Setup, Complete..Ready!";
String WindowMsg = "A Button Has Been Pressed";
/*************************************************/
int MagSens1 = 4;
int MagSens2 = 5;
int MagSens3 = 6;
/*************************************************/
int MagState = 0;

void setup()
{
/*************************************************/
pinMode(MagSens1, INPUT);
pinMode(led,OUTPUT);
/*************************************************/
Serial.begin(9600);  
Serial1.begin(115200);// the GPRS baud rate 
//Serial1.println("AT+CLCK=\"SC\",0,\"3306\"");
digitalWrite(led,HIGH);
delay(1000);
msg = Startup;
SendTextMessage();
} 

void loop()
{
 /* MagState = digitalRead(MagSens1);
  
  if (MagState == LOW)
  {
     //msg = t;
     msg = WindowMsg;
     SendTextMessage();
  }
 */
 

 ReceiveTextMessage();
 delay(5000);
 
}


/*****************************************************/
void SendTextMessage()
{
Serial1.print("AT+CMGF=1\r"); //Because we want to send the SMS in text mode
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
Serial1.println("AT+CMGD=\"ALL\"");
//Serial1.println("AT+CMGD=0,4");
delay(1000);
Serial1.println("AT+CMGL=\"ALL\""); // show message
//Serial1.println("AT+CMGR=1");
t = Serial1.read();
      Serial.print(Serial1.read());

/*while(Serial1.available()>0) {
      t = Serial1.read();
      Serial.println("Message Received : ");
      Serial.print(t);
      if(t == 'o'){
        t = Serial1.read();
        if(t == 'n'){
        
      digitalWrite(led,LOW);
      Serial.print("On");
      delay(6000);
      digitalWrite(led,HIGH);
      
        }
        else if(t != 'n')
        break;
      }
  }
  */
Serial.println("\nEND OF MESSAGE");

} 
