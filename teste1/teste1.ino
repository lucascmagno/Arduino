#define pinPIR 7

void setup()
{
  pinMode(pinPIR, INPUT);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}

void loop()
{
  bool valorPIR = digitalRead(pinPIR);

  if(valorPIR){
    delay(30);
    digitalWrite(8, HIGH);
    delay(700);
    digitalWrite(8, LOW);
    delay(700);
    Serial.println("DETECTADO");
  }else{
    Serial.println("----");
    delay(30);
    digitalWrite(8, LOW);
  }
}
