const int encoderPinA = 6;
const int encoderPinB = 7;
const int encoderBtn = 8;
int encoderPos = 0;
boolean encoderALast = LOW;  // remembers the previous pin state
unsigned long time = 0;
long count = 0; //num counter
long num = 0;

void revEncoder_init()
{
   pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderBtn, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
}
void revEncoder_judge()
{
   boolean encoderA = digitalRead(encoderPinA);
  if (digitalRead(encoderBtn) == LOW)
  {
    delay(10);
    if (digitalRead(encoderBtn) == LOW)
    {
      Serial.println("Button press.");
      delay(10);
    }
    while(digitalRead(encoderBtn) == LOW);
  }
  
  if ((encoderALast == HIGH) && (encoderA == LOW))
  {
    
    if (digitalRead(encoderPinB) == LOW)
    {
      encoderPos--;
    }
    else if(digitalRead(encoderPinB) == HIGH)
    {
      encoderPos++;
    }
    Serial.println (encoderPos);
    lcd.setCursor(12,1);
    lcd.print("    ");
    lcd.setCursor(12,1);
  lcd.print(encoderPos);
  }
  encoderALast = encoderA;


  
}
