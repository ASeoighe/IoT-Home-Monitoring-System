#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>
//#include <SoftwareSerial.h>
#include <String.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

int i, n;
int Relay1 = 4;
int Relay2 = 5;
int Relay3 = 6;

float tempC;
int reading;
int LDR_Read;
int tempPin = A0;
int LDR = A1;

String msg;
String MotionDetected = "Movement Has Been Detected!";
String Startup = "Setup and PIR Calibration Complete..Ready for commands!";
String DoorMsg = "Door Has Been Opened";

byte mac[] = {0x98, 0x4F, 0xEE, 0x01, 0x81, 0xEA};
IPAddress ip(192, 168, 137, 10);
char* SensReadOut[] = {"OFF", "ON"};
EthernetServer server(80);
// Store the LED's status (1: on; 0: off)
int Relay1Status = 0;
int Relay2Status = 0;
int Relay3Status = 0;
int Relay4Status = 0;
int BackdoorLight = 13;

// String to store the incoming HTTP request
String HTTPRequest;

char password[] = {1, 2, 3, 4};
char userInput[4];

int calibrationTime = 20; //the time we give the sensor to calibrate (10-60 secs according to the datasheet)

long unsigned int lowIn; //the time when the sensor outputs a low impulse

//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
//long int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

int pirRoom1 = 8;    //the digital pin connected to the PIR sensor's output
int pirRoom2 = 9;
int pirRoom3 = 10;
int BackPir = 11;
int doorSwitch = 12;

int pirState = LOW;             // we start, assuming no motion detected
int pir2State = LOW;
int pir3State = LOW;
int pirbackState = LOW;
int val = 0;                    // variable for reading the pin status



char KEYS[] = {
  '1', '2', '3',
  '4', '5', '6',
  '7', '8', '9',
  '*', '0', '#',
};

OnewireKeypad <Print, 12 > Keypad(Serial, KEYS, 4, 3, A0, 4700, 1000 );


void setup() {
  //######  INPUTS ######
  pinMode(pirRoom1, INPUT);
  pinMode(pirRoom2, INPUT);
  pinMode(pirRoom3, INPUT);
  pinMode(BackPir, INPUT);

  //######  OUTPUTS ######
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(BackdoorLight, OUTPUT);




  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);

  Serial.begin(9600);
  Serial1.begin(115200);// the GSM baud rate

  Serial1.println("AT+CMGF=1");
  delay(2000);
  Serial1.println("AT+CMGD=1,4");
  delay(2000);

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(BackdoorLight, LOW);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  Serial.println("Ready For Message");
  msg = Startup;
  SendTextMessage();

}

void loop() {

  LDR_Read = analogRead(LDR);
  Serial.print("LDR Reading is: ");
  Serial.println(LDR_Read);

  if (LDR_Read < 60) {
    digitalWrite(BackdoorLight, HIGH);
    delay(5000);
  }
  else
    digitalWrite(BackdoorLight, LOW);

  int doorState = digitalRead(doorSwitch);
  Serial.print("Your Door is ");
  Serial.println(doorState);
  if (doorState == LOW)
  {
    digitalWrite(BackdoorLight, HIGH);
    Serial.println("Door has been opened");
    msg = "Back door has been opened";
    SendTextMessage();
  }
  else if (BackdoorLight, HIGH) {
    digitalWrite(BackdoorLight, LOW);
  }
  ReceiveTextMessage();
  delay(100);
  EthernetClient client = server.available();
  // Serve web page & handle client requests
  if (client) {
    Serial.println("New client connected");
    WebCheck(client);
  }
  // Close the connection
  client.stop();
  PIRSens();

}


//%%%%%%%%%%%%%%%%%%%%%%{FUNCTIONS}%%%%%%%%%%%%%%%%%%%%

/*
void Keypad() {

  Keypad.SetHoldTime(100);  // Key held time in ms
  Keypad.SetDebounceTime(50);

          Serial.println("Enter Password");

         for(i=1;i<5;i++){
           Serial.print("Enter Digit ");
           Serial.println(i);
           while(Keypad.Key_State() == 0) {}
          // data = serialRead();
          if((Keypad.Key_State() == 3))
           userInput[i] = Keypad.Getkey();
           Serial.print(userInput[i]);
           while ((Keypad.Key_State())) {}

           }

         Serial.print("Password Entered is: ");
         for(i=0;i<4;i++){Serial.print(userInput[i]);}

         for(i=0;i<4;i++){

         }

}
*/
void PIRSens() {
  //############################ ROOM 1 ################################

  val = digitalRead(pirRoom1);  // read input value
  if (val == LOW) {            // check if the input is LOW
    //digitalWrite(Relay1, LOW);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion ended in room 1");
      msg = "Movement Detected in Room1";
      SendTextMessage();
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  }
  else {
    //digitalWrite(Relay1, HIGH); // turn LED OFF
    if (pirState == HIGH) {
      // we have just turned off
      Serial.println("Motion Detected in room 1");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  //################################ ROOM 2 #############################

  int pir2val = digitalRead(pirRoom2);  // read input value
  if (pir2val == LOW) {            // check if the input is LOW
    if (pir2State == HIGH) {
      // we have just turned off
      Serial.println("Motion ended in room2 !");
      // We only want to print on the output change, not state
      pir2State = LOW;
    }
  }

  else {

    if (pir2State == LOW) {
      // we have just turned on
      Serial.println("Motion detected in room2 ");
      //msg = "Movement Detected in Room2";
      //SendTextMessage();
      // We only want to print on the output change, not state
      pir2State = HIGH;
    }
  }
  //################################# ROOM 3 ####################################

  /*

   int pir3val = digitalRead(pirRoom3);  // read input value
  if (pir3val == LOW) {            // check if the input is LOW
    if (pir3State == HIGH){
     // we have just turned off
     Serial.println("Motion ended in room3 !");
     // We only want to print on the output change, not state
     pir3State = LOW;
    }
    }

  else {

    if (pir3State == LOW) {
     // we have just turned on
     Serial.println("Motion detected in room3 ");
     // We only want to print on the output change, not state
     pir3State = HIGH;
    }
  }
    //################################# Back Door #############################

  //if(LDR < 30){
  do{
    //Serial.println(LDR_Read);
   int BackPirVal = digitalRead(pirRoom2);  // read input value
  if (pir2val == LOW) {            // check if the input is LOW
    if (pir2State == HIGH){
     // we have just turned off
     Serial.println("Motion ended in room2 !");
     // We only want to print on the output change, not state
     pir2State = LOW;
    }
    }

  else {

    if (pir2State == LOW) {
     // we have just turned on
     Serial.println("Motion detected in room2 ");
     //msg = "Movement Detected in Room2";
     //SendTextMessage();
     // We only want to print on the output change, not state
     pir2State = HIGH;
    }
  }
  }
  while(LDR_Read <50);
  */
}

void SendTextMessage() {
  Serial1.println("AT+CMGS=\"+353876373466\"");
  delay(2000);
  Serial1.println(msg);
  delay(1000);
  Serial1.write(0x1A);//the ASCII code of the ctrl+z
  delay(1000);
  Serial1.println();
}

void ReceiveTextMessage() {
  char t;

  Serial1.println("AT+CPMS=\"SM\"");  // Lists SMS
  delay(1000);
  Serial1.println("AT+CMGR=1"); // Reads SMS Address 1
  delay(1000);

  while (Serial1.available() > 0) {
    t = Serial1.read();
    Serial.print(t);
    if (t == 'o' || t == 'O') {
      t = Serial1.read();
      if (t == 'n') {
        n = Serial1.read();
        if (n == '1') {
          digitalWrite(Relay1, LOW);
          Relay1Status = 1;
        }
        else if (n == '2') {
          digitalWrite(Relay2, LOW);
          Relay2Status = 1;
        }
        else if (n == '3') {
          digitalWrite(Relay3, LOW);
          Relay3Status = 1;
        }

        else if (n == 'a' || n == 'A') {
          digitalWrite(Relay1, LOW);
          digitalWrite(Relay2, LOW);
          digitalWrite(Relay3, LOW);
          //digitalWrite(Relay4, LOW);
          //digitalWrite(Relay5, LOW);
          Relay1Status = 1;
          Relay2Status = 1;
          Relay3Status = 1;
          Serial1.println("AT+CMGD=1,4");
          delay(500);
        }
        delay(1000);
        Serial1.println("AT+CMGD=1,4");
        delay(500);
        break;
        Serial.print("End of Messgage");
      }
      else if (t != 'n' && t == 'f')
      {
        t = Serial1.read();
        if (t = 'f')
        {

          n = Serial1.read();
          if (n == '1') {
            digitalWrite(Relay1, HIGH);
            Relay1Status = 0;
          }
          else if (n == '2') {
            digitalWrite(Relay2, HIGH);
            Relay2Status = 0;

          }
          else if (n == '3') {
            digitalWrite(Relay3, HIGH);
            Relay3Status = 0;
          }

          else if (n == 'a' || n == 'A') {
            digitalWrite(Relay1, HIGH);
            digitalWrite(Relay2, HIGH);
            digitalWrite(Relay3, HIGH);
            // digitalWrite(Relay4, HIGH);
            //digitalWrite(Relay5, HIGH);
            Relay1Status = 0;
            Relay2Status = 0;
            Relay3Status = 0;
            Serial1.println("AT+CMGD=1,4");
            delay(500);
          }
          //delay(1000);
          Serial1.println("AT+CMGD=1,4");
          delay(500);
          Serial.println("End of Messgage");
        }
        else {
          break;
        }
      }
    }
  }
  //Serial.println("\nEnd of Loop");
}


void WebCheck(EthernetClient &client) {
  reading = analogRead(tempPin);
  tempC = reading / 9.31;
  Serial.print("Relay 1 Status: ");
  Serial.println(Relay1Status);
  Serial.println(tempC);
  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();

      HTTPRequest += c;

      Serial.write(c);
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        // send a standard http response header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        // add a meta refresh tag, so the browser pulls again every 5 seconds:
        client.println("<meta http-equiv=\"refresh\" content=\"5\">");
        // output the value of each analog input pin
        /***************************  HEAD  ************************/
        client.println("<head> ");
        client.println("<style type=\"text/css\">table{font-family:arial;}</style>");

        client.print("<title>Intel Galileo Project</title>");
        client.print("</title>");
        client.println("</header>");
        /*************************************************************/

        /***************************** BODY ****************************/
        client.println("<body bgcolor=\"#6495ED\">");
        //client.println("<body>");
        client.println("<h1><center>Galileo Project</center></h1>");
        client.println("<h2><center>Home Monitoring & Control System</center></h2><hr />");
        client.print("<h3><center>House Temperature is ");
        client.print(tempC);
        client.println(" Celcius</center></h3><hr />");
        client.println("<center><strong><table border=\".3\" width=\"50%\" cellpadding=\"10\">");
        client.println("<tr><th colspan=\"2\"><h3>Room</h3></th><th><h3>State</strong></h3></tr>");

        // output the value of each analog input pin
        for (int digitalChannel = 4; digitalChannel <= 6; digitalChannel++) {
          int sensorReading = digitalRead(digitalChannel);
          client.println("<tr>");
          client.print("<td colspan=\"2\">");

          client.print("Room ");
          client.print(digitalChannel - 3);
          client.print(" is ");
          client.println("</td>");
          if (sensorReading > 0)
          {
            client.print("<td><center><input type=\"button\" id=\"");
            client.print(digitalChannel - 3);
            client.print("\"value=\"");
            client.print(SensReadOut[0]);
            client.print("\"");
          }
          else
          {
            client.print("<td><center><input type=\"button\"value=\"");
            client.print(SensReadOut[1]);
            client.print("\"");
          }

          client.print("onclick=\"this.value=this.value=='ON'?'OFF':'ON';\"></input></center></td>");
          client.println("</tr>");

          client.println("<br />");
        }


        client.print("</table></strong></center>");
        // Use a HTML form to get the user input via a checkbox

        client.print("<center>");
        client.println("<form method=\"get\">");
        // First check if the checkbox has been clicked
        if (HTTPRequest.indexOf("GET /?Relay1=2") > -1)
          Relay1Status = 1;
        else if (HTTPRequest.indexOf("GET /?") > -1)
          Relay1Status = 0;

        if (HTTPRequest.indexOf("GET /?Relay2=3") > -1)
          Relay2Status = 1;
        else if (HTTPRequest.indexOf("GET /?") > -1)
          Relay2Status = 0;
        if (HTTPRequest.indexOf("GET /?Relay3=4") > -1)
          Relay3Status = 1;
        else if (HTTPRequest.indexOf("GET /?") > -1)
          Relay3Status = 0;

        if (HTTPRequest.indexOf("GET /?Relay2=3&Relay3=4") > -1) {
          Relay2Status = 1;
          Relay3Status = 1;
        }
        if (HTTPRequest.indexOf("GET /?Relay1=2&Relay2=3") > -1) {
          Relay1Status = 1;
          Relay2Status = 1;
        }
        if (HTTPRequest.indexOf("GET /?Relay1=2&Relay3=4") > -1) {
          Relay1Status = 1;
          Relay3Status = 1;
        }
        if (HTTPRequest.indexOf("GET /?Relay1=2&Relay2=3&Relay3=4") > -1) {
          Relay1Status = 1;
          Relay2Status = 1;
          Relay3Status = 1;
        }


        // Display the checked/unchecked box on the webpage as requested
        // & turn on/off the LED on the board at the same time
        if (Relay1Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay1\" value=\"2\" onclick=\"submit();\" checked>Relay1");
          digitalWrite(Relay1, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay1\" value=\"2\" onclick=\"submit();\">Relay1");
          digitalWrite(Relay1, HIGH);
        }
        if (Relay2Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay2\" value=\"3\" onclick=\"submit();\" checked>Relay2");
          digitalWrite(Relay2, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay2\" value=\"3\" onclick=\"submit();\">Relay2");
          digitalWrite(Relay2, HIGH);
        }
        if (Relay3Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay3\" value=\"4\" onclick=\"submit();\" checked>Relay3");
          digitalWrite(Relay3, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay3\" value=\"4\" onclick=\"submit();\">Relay3");
          digitalWrite(Relay3, HIGH);
        }

        // Write the closing HTML tags to the webpage
        client.println("</form></center>");
        client.println("</body>");
        client.print("<footer><hr /><em>Aaron Scally Joyce B.Eng Comp Electronic Eng G.M.I.T</em></footer>");
        client.println("</html>");

        HTTPRequest = "";

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
  //client.stop();
  //Serial.println("client disonnected");
}



