#define TransmitPin 5  //RF Transmitter pin

int sw = 7;
int swstate = 0;

int led = 13;

 void setup(){
   pinMode(TransmitPin, OUTPUT);   
   pinMode(sw, INPUT);
   pinMode(led, OUTPUT);
 }

 void loop(){
   swstate = digitalRead(sw);
  
   if(swstate == HIGH)
   {
   digitalWrite(TransmitPin, LOW);  
   digitalWrite(led, LOW); 
   }
   else
   {
   digitalWrite(TransmitPin, HIGH);
    digitalWrite(led, HIGH); 
   }
 }
