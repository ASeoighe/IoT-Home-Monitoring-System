#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

int i, n;
int Relay1 = 4;
int Relay2 = 5;
int Relay3 = 6;
int Relay4 = 7;
int Relay5 = 8;

String msg;
String Startup = "Setup, Complete..Ready!";
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
// String to store the incoming HTTP request
String HTTPRequest;  


void setup()
{
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  // Open serial communications
  Serial.begin(9600);
  // Delay to give time to open the Serial console
  delay(2000);
  
  // Start the Ethernet connection and the server
  Ethernet.begin(mac, ip);
  server.begin();
  
  // Display Server IP
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
}

void loop()
{
  // Listen for incoming clients
  EthernetClient client = server.available();
  // Serve web page & handle client requests
  if(client) {
    Serial.println("New client connected");
    handleClient(client);
  }
  // Close the connection
  client.stop();
}


void handleClient(EthernetClient &client)
{
  boolean currentLineIsBlank = true;
  // Run while the client is connected
  while (client.connected()) {
    // If client data is available to read
    if(client.available()) {
      // Read 1 byte (1 character) from client
      char c = client.read();
      // Add the data to the String
      HTTPRequest += c;
      // Echo characters to Serial
      Serial.write(c);
      // The last line of a client request is blank and ends with \n
      // Respond to client only after last line has been received
      if (c == '\n' && currentLineIsBlank) {
        // Send a standard http response header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
          
        // Serve the web page to the Client. This involves writing some HTML to the browser. I used CoffeeCup 
        // to see what my HTML webpage looked like, before wrapping each statement in client.println() calls.  
        // First write the opening HTML tags and browser tab title and webpage headings
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head>");
        // Add a meta refresh tag, so the browser pulls again every 5 seconds:
        client.println("<meta http-equiv=\"refresh\" content=\"5\">");
        client.println("<title>Galileo Project</title>");
        client.println("</head>");
        // You can set the background and text colours using strings
        client.println("<body bgcolor=\"white\" text=\"navy\">");
        client.println("<h1>Galileo Project</h1>");
        client.println("<p>Turn Relays on/off</p>");
        
        // Use a HTML form to get the user input via a checkbox
        client.println("<form method=\"get\">");
        // First check if the checkbox has been clicked  
        if(HTTPRequest.indexOf("GET /?Relay1=2") > -1)
          Relay1Status = 1;
         else if(HTTPRequest.indexOf("GET /?") > -1)
          Relay1Status = 0;
          
        if(HTTPRequest.indexOf("GET /?Relay2=3") > -1)
          Relay2Status = 1;
        else if(HTTPRequest.indexOf("GET /?") > -1)
          Relay2Status = 0;
        if(HTTPRequest.indexOf("GET /?Relay3=4") > -1)
          Relay3Status = 1;
        else if(HTTPRequest.indexOf("GET /?") > -1)
          Relay3Status = 0;
          
        if(HTTPRequest.indexOf("GET /?Relay2=3&Relay3=4") > -1){
          Relay2Status = 1;
          Relay3Status = 1;
        }
        if(HTTPRequest.indexOf("GET /?Relay1=2&Relay2=3") > -1){
          Relay1Status = 1;
          Relay2Status = 1;
        }
        if(HTTPRequest.indexOf("GET /?Relay1=2&Relay3=4") > -1){
          Relay1Status = 1;
          Relay3Status = 1;
        }
        if(HTTPRequest.indexOf("GET /?Relay1=2&Relay2=3&Relay3=4") > -1){
          Relay1Status = 1;
          Relay2Status = 1;
          Relay3Status = 1;
        }
          

        // Display the checked/unchecked box on the webpage as requested 
        // & turn on/off the LED on the board at the same time  
        if(Relay1Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay1\" value=\"2\" onclick=\"submit();\" checked>Relay1");
          digitalWrite(Relay1, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay1\" value=\"2\" onclick=\"submit();\">Relay1");
          digitalWrite(Relay1, HIGH);
        }
        if(Relay2Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay2\" value=\"3\" onclick=\"submit();\" checked>Relay2");
          digitalWrite(Relay2, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay2\" value=\"3\" onclick=\"submit();\">Relay2");
          digitalWrite(Relay2, HIGH);
        }
        if(Relay3Status == 1) {
          client.println("<input type=\"checkbox\" name=\"Relay3\" value=\"4\" onclick=\"submit();\" checked>Relay3");
          digitalWrite(Relay3, LOW);
        }
        else {
          client.println("<input type=\"checkbox\" name=\"Relay3\" value=\"4\" onclick=\"submit();\">Relay3");
          digitalWrite(Relay3, HIGH);
        }
        
        // Write the closing HTML tags to the webpage
        client.println("</form>");
        client.println("</body>");
        client.println("</html>");
        
        // Reset the HTTPRequest string
        HTTPRequest = "";
        
        break;
      } // end if (c == '\n' && currentLineIsBlank)
      
      // Check for the end of a line. Every line of text ends with \r\n
      if(c == '\n') {
        // Reset the flag ready for the new line
        currentLineIsBlank = true;
      }
      else if(c != '\r') {
        // A text character was received
        currentLineIsBlank = false;
      }
    } // end if (client.available())
  } // end while (client.connected())
    
  // Give the web browser time to receive the data
  delay(1);
}

