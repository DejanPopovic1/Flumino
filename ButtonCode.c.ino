void evaluateButton1(const int button1State, int *lastButton1State, int *button1PushCounter, const int show_dose){
  if (button1State != *lastButton1State && button1State == 0)
  {
    if (*button1PushCounter == 8)
    {
      *button1PushCounter = 0;
    }
    else if(*button1PushCounter == 2 && show_dose == false)
    {
      *button1PushCounter = 8;
    }
    else
    {
      Serial.println("Hello!");
      (*button1PushCounter)++;
    }
  }
  *lastButton1State = button1State;
}
