#define HIGH 1000
#define LOW 0

void evaluateButton1(const int button1State, int *lastButton1State, int *button1PushCounter, const int show_dose) {
  if (*lastButton1State != LOW && button1State == LOW) {
    if (*button1PushCounter == 8)
    {
      *button1PushCounter = 0;
    }
    else if (*button1PushCounter == 2 && show_dose == false)
    {
      *button1PushCounter = 8;
    }
    else
    {
      (*button1PushCounter)++;
    }
  }
  *lastButton1State = button1State;
}

void evaluateButton2(const int button2State, int *lastButton2State, const int Menu, int *const Case0Count, int *const Case1Count, int *const Case2CountMg, int *const Case2CountUg, int *const Case3Count,
                     int *const Case4Count, int *const Case5Count, int *const show_dose, int *const dose_shown) {
  if (*lastButton2State != LOW && button2State == LOW) {
    if (Menu == 0)
      if (*Case0Count >= 2)
        *Case0Count = 0;
      else
        (*Case0Count)++;

    else if (Menu == 4)
      if (*Case1Count >= 1)
        *Case1Count = 0;
      else
        (*Case1Count)++;

    else if (Menu == 5 && *Case1Count == 0)
      if (*Case2CountUg >= 100)
        *Case2CountUg = 0;

      else
        (*Case2CountUg)++;

    else if (Menu == 5 && *Case1Count == 1)
      if (*Case2CountMg >= 100)
        *Case2CountMg = 0;
      else
        (*Case2CountMg)++;

    else if (Menu == 6)
      if (*Case3Count >= 500)
        *Case3Count = 0;
      else
        (*Case3Count)++;

    else if (Menu == 7)
      if (*Case4Count >= 1000)
        *Case4Count = 0;
      else
        (*Case4Count)++;

    else if (Menu == 1)
      if (*Case5Count >= 100)
        *Case5Count = 0;
      else
        (*Case5Count)++;

    else if (Menu == 2)
      if (*show_dose == true)
        *show_dose = false;
      else
        *show_dose = true;

    else if (Menu == 3)
      if (*dose_shown >= 2)
        *dose_shown = 0;
      else
        (*dose_shown)++;
    delay(1);
  }
  *lastButton2State = button2State;
}

void evaluateButton3(const int button3State, int *lastButton3State, const int Menu, int *const Case0Count, int *const Case1Count, int *const Case2CountMg, int *const Case2CountUg, int *const Case3Count,
                     int *const Case4Count, int *const Case5Count, int *const show_dose, int *const dose_shown) {
  if (*lastButton3State != LOW && button3State == LOW) {
    if (Menu == 0)
      if (*Case0Count == 0)
        *Case0Count = 2;
      else
        (*Case0Count)--;

    else if (Menu == 4)
      if (*Case1Count == 0)
        *Case1Count = 1;
      else
        (*Case1Count)--;

    else if (Menu == 5 && *Case1Count == 0)
      if (*Case2CountUg == 0)
        *Case2CountUg = 100;
      else
        (*Case2CountUg)--;
    else if (Menu == 5 && *Case1Count == 1)
      if (*Case2CountMg == 0)
        *Case2CountMg = 100;
      else
        (*Case2CountMg)--;

    else if (Menu == 6)
      if (*Case3Count == 0)
        *Case3Count = 500;
      else
        (*Case3Count)--;

    else if (Menu == 7)
      if (*Case4Count == 0)
        *Case4Count = 1000;
      else
        (*Case4Count)--;

    else if (Menu == 1)
      if (*Case5Count == 0)
        *Case5Count = 100;
      else
        (*Case5Count)--;

    else if (Menu == 2)
      if (*show_dose == true)
        *show_dose = false;
      else
        *show_dose = true;

    else if (Menu == 3)
      if (*dose_shown == 0)
        *dose_shown = 2;
      else
        (*dose_shown)--;
    delay(1);
  }
  *lastButton3State = button3State;
}

void evaluateButton4(const int button4State, int *const lastButton4State, bool *const BuzzerState, float *const lower_sound_thresh, float *const upper_sound_thresh, float *const lower_drugsound_thresh,
                     float *const upper_drugsound_thresh, const double newFlowRate, const double DrugFlowRate, const int Case5Count) {
  if (*lastButton4State != LOW && button4State == LOW) {
    *BuzzerState = !(*BuzzerState);
    *lower_sound_thresh = newFlowRate * (1 - (float)Case5Count / 100);
    *upper_sound_thresh = newFlowRate * (1 + (float)Case5Count / 100);
    *lower_drugsound_thresh = DrugFlowRate * (1 - (float)Case5Count / 100);
    *upper_drugsound_thresh = DrugFlowRate * (1 + (float)Case5Count / 100);
  }
  *lastButton4State = button4State;
}

void evaluateBuzzer(const bool BuzzerState, const double newFlowRate, const float lower_sound_thresh, const float upper_sound_thresh, const int BuzzerPin) {
  if (BuzzerState && (newFlowRate < lower_sound_thresh  || newFlowRate > upper_sound_thresh)) {
    digitalWrite(BuzzerPin, HIGH );
  }
  else {
    digitalWrite(BuzzerPin, LOW );
  }
}
