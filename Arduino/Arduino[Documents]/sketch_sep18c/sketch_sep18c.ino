
int x = A0;
int xvalue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  xvalue = analogRead(x);

  Serial.println(xvalue);

}
