#include "Intergrate_attemp.h"
#include "Arduino.h"
#include "Main.cpp"

void Text::Send()
{
  Serial1.println("AT+CMGS=\"+353876373466\"");
  delay(1000);
  Serial1.println(msg);
  delay(1000);
  Serial1.write(0x1A);//the ASCII code of the ctrl+z
  delay(1000);
  Serial1.println();
}

void Text::Receive()
{
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
        }
        else if (n == '2') {
          digitalWrite(Relay2, LOW);
        }
        else if (n == '3') {
          digitalWrite(Relay3, LOW);
        }
        else if (n == '4') {
          digitalWrite(Relay4, LOW);
        }
        else if (n == '5') {
          digitalWrite(Relay5, LOW);
        }
        else if (n == 'a' || n == 'A') {
          digitalWrite(Relay1, LOW);
          digitalWrite(Relay2, LOW);
          digitalWrite(Relay3, LOW);
          //digitalWrite(Relay4, LOW);
          //digitalWrite(Relay5, LOW);
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
          }
          else if (n == '2') {
            digitalWrite(Relay2, HIGH);
          }
          else if (n == '3') {
            digitalWrite(Relay3, HIGH);
          }
          else if (n == '4') {
            digitalWrite(Relay4, HIGH);
          }
          else if (n == '5') {
            digitalWrite(Relay5, HIGH);
          }
          else if (n == 'a' || n == 'A') {
            digitalWrite(Relay1, HIGH);
            digitalWrite(Relay2, HIGH);
            digitalWrite(Relay3, HIGH);
           // digitalWrite(Relay4, HIGH);
            //digitalWrite(Relay5, HIGH);
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



void Web::Display(EthernetClient &client)
{
    
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
          client.println("<center><strong><table border=\".3\" width=\"50%\" cellpadding=\"10\">");
          client.println("<tr><th colspan=\"2\"><h3>Room</h3></th><th><h3>State</strong></h3></tr>");
      
          // output the value of each analog input pin
          for (int analogChannel = 4; analogChannel <= 6; analogChannel++) {
            int sensorReading = digitalRead(analogChannel);
            client.println("<tr>");
            client.print("<td colspan=\"2\">");
             
            client.print("Relay");
            client.print(analogChannel - 3);
            client.print(" is ");
            client.println("</td>");
            if (sensorReading > 0)
            {
             client.print("<td><center><input type=\"button\" id=\"");
             client.print(analogChannel - 3);
             client.print("\"value=\"");
             client.print(SensReadOut[0]);
             client.print("\"");
             //client.read("");
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
    client.stop();
    //Serial.println("client disonnected");
  }





