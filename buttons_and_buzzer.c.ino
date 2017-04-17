#define MAXIMUM_CYCLE_TIME  20000
#define DISPLAY_PERIOD      2000000

bool evaluateSensor(const int previoiusSensorReading, const int sensorReading) {
  if (previoiusSensorReading == HIGH && sensorReading == LOW) {
    return true;
  }
  else {
    return false;
  }
}

void evaluateFlowRate(bool isDropPassing, double *DisplayedFlowRate, unsigned long *previousTime, unsigned long currentTime, int dropsPerMillilitre, double *newFlowRate, unsigned long *period) {
  if (isDropPassing == true) {
    *newFlowRate = flowRate(previousTime, currentTime, dropsPerMillilitre, newFlowRate, period);
  }
  if (currentTime >= *previousTime + *period) {
    *newFlowRate =  decayedFlowRate(*previousTime, currentTime, dropsPerMillilitre, *period);
  }
  if (currentTime % DISPLAY_PERIOD <= MAXIMUM_CYCLE_TIME) {
    *DisplayedFlowRate = *newFlowRate;
  }
}

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

void evaluateButton2(const int button2State, int *lastButton2State, const int Menu, int *const dropsPerMillilitreSelector, int *const inputDrugMassUOMSelector, int *const drugMassMgSelector, int *const drugMassUgSelector, int *const patientMassSelector,
                     int *const volumeDilutantSelector, int *const allowableFlowRateSelector, int *const show_dose, int *const dose_shown, int *const dropsPerMillilitre) {
  if (*lastButton2State != LOW && button2State == LOW) {
    if (Menu == 0) {
      if (*dropsPerMillilitreSelector >= 2)
        *dropsPerMillilitreSelector = 0;
      else
        (*dropsPerMillilitreSelector)++;
      switch (*dropsPerMillilitreSelector) {
        case 0:
          *dropsPerMillilitre = 10;
          break;
        case 1:
          *dropsPerMillilitre = 20;
          break;
        case 2:
          *dropsPerMillilitre = 60;
          break;
      }
    }
    else if (Menu == 4)
      if (*inputDrugMassUOMSelector >= 1)
        *inputDrugMassUOMSelector = 0;
      else
        (*inputDrugMassUOMSelector)++;

    else if (Menu == 5 && *inputDrugMassUOMSelector == 0)
      if (*drugMassUgSelector >= 100)
        *drugMassUgSelector = 0;

      else
        (*drugMassUgSelector)++;

    else if (Menu == 5 && *inputDrugMassUOMSelector == 1)
      if (*drugMassMgSelector >= 100)
        *drugMassMgSelector = 0;
      else
        (*drugMassMgSelector)++;

    else if (Menu == 6)
      if (*patientMassSelector >= 500)
        *patientMassSelector = 0;
      else
        (*patientMassSelector)++;

    else if (Menu == 7)
      if (*volumeDilutantSelector >= 1000)
        *volumeDilutantSelector = 0;
      else
        (*volumeDilutantSelector)++;

    else if (Menu == 1)
      if (*allowableFlowRateSelector >= 100)
        *allowableFlowRateSelector = 0;
      else
        (*allowableFlowRateSelector)++;

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

void evaluateButton3(const int button3State, int *lastButton3State, const int Menu, int *const dropsPerMillilitreSelector, int *const inputDrugMassUOMSelector, int *const drugMassMgSelector, int *const drugMassUgSelector, int *const patientMassSelector,
                     int *const volumeDilutantSelector, int *const allowableFlowRateSelector, int *const show_dose, int *const dose_shown, int *const dropsPerMillilitre) {
  if (*lastButton3State != LOW && button3State == LOW) {
    if (Menu == 0) {
      if (*dropsPerMillilitreSelector == 0)
        *dropsPerMillilitreSelector = 2;
      else
        (*dropsPerMillilitreSelector)--;
      switch (*dropsPerMillilitreSelector) {
        case 0:
          *dropsPerMillilitre = 10;
          break;
        case 1:
          *dropsPerMillilitre = 20;
          break;
        case 2:
          *dropsPerMillilitre = 60;
          break;
      }
    }
    else if (Menu == 4)
      if (*inputDrugMassUOMSelector == 0)
        *inputDrugMassUOMSelector = 1;
      else
        (*inputDrugMassUOMSelector)--;

    else if (Menu == 5 && *inputDrugMassUOMSelector == 0)
      if (*drugMassUgSelector == 0)
        *drugMassUgSelector = 100;
      else
        (*drugMassUgSelector)--;
    else if (Menu == 5 && *inputDrugMassUOMSelector == 1)
      if (*drugMassMgSelector == 0)
        *drugMassMgSelector = 100;
      else
        (*drugMassMgSelector)--;

    else if (Menu == 6)
      if (*patientMassSelector == 0)
        *patientMassSelector = 500;
      else
        (*patientMassSelector)--;

    else if (Menu == 7)
      if (*volumeDilutantSelector == 0)
        *volumeDilutantSelector = 1000;
      else
        (*volumeDilutantSelector)--;

    else if (Menu == 1)
      if (*allowableFlowRateSelector == 0)
        *allowableFlowRateSelector = 100;
      else
        (*allowableFlowRateSelector)--;

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

void evaluateButton4(const int menu, const int button4State, int *const lastButton4State, bool *const BuzzerState, float *const lower_sound_thresh, float *const upper_sound_thresh, float *const lower_drugsound_thresh,
                     float *const upper_drugsound_thresh, const double newFlowRate, const double DrugFlowRate, const int allowableFlowRateSelector) {
  if (*lastButton4State != LOW && button4State == LOW && Menu == flow_rate_page) {
    *BuzzerState = !(*BuzzerState);
    *lower_sound_thresh = newFlowRate * (1 - (float)allowableFlowRateSelector / 100);
    *upper_sound_thresh = newFlowRate * (1 + (float)allowableFlowRateSelector / 100);
    *lower_drugsound_thresh = DrugFlowRate * (1 - (float)allowableFlowRateSelector / 100);
    *upper_drugsound_thresh = DrugFlowRate * (1 + (float)allowableFlowRateSelector / 100);
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
