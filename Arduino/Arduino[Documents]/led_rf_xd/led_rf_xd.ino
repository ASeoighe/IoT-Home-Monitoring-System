const int analoginpin = A0;
int led = 13;
float sensorVal = 0;

int certainty = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

void loop()
{
  sensorVal = analogRead(analoginpin);

  Serial.print("Voltage Output = ");
  Serial.print(sensorVal*5/1023);
  Serial.println(" ");

if (sensorVal < 230)
{
  certainty++;
}
else
{
  certainty = 0;
}

if(certainty >= 7)
{
  digitalWrite(led,HIGH);
}
else
{
  digitalWrite(led,LOW);
}
  delay(100);
}


