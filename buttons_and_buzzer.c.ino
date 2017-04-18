#define MAXIMUM_CYCLE_TIME                  20000
#define DISPLAY_PERIOD                      2000000

#define ALARM_DEVIATION_LOWER_BOUND         1
#define ALARM_DEVIATION_UPPER_BOUND         100
#define DRUG_UG_MASS_DEVIATION_LOWER_BOUND  1
#define DRUG_UG_MASS_DEVIATION_UPPER_BOUND  100
#define DRUG_MG_MASS_DEVIATION_LOWER_BOUND  1
#define DRUG_MG_MASS_DEVIATION_UPPER_BOUND  100
#define PATIENT_MASS_LOWER_BOUND            1
#define PATIENT_MASS_UPPER_BOUND            500
#define VOLUME_DILUTANT_LOWER_BOUND         1
#define VOLUME_DILUTANT_UPPER_BOUND         1000

#define BUTTON_HELD_TRIGGER_INCREMENT_TIME 500000
#define BUTTON_HELD_INCREMENT_TIME 500000
#define BUTTON_HELD_INCREMENT_TIME_ALARM_DEVIATION 1
#define BUTTON_HELD_INCREMENT_DRUG_MASS_UG 10
#define BUTTON_HELD_INCREMENT_DRUG_MASS_MG 10
#define BUTTON_HELD_INCREMENT_PATIENT_MASS 5
#define BUTTON_HELD_INCREMENT_VOLUME_DILUTANT 50

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

void evaluateButton2(const int button2State, int *lastButton2State, const int Menu, int *const dropsPerMillilitreSelector, int *const inputDrugMassUOMSelector, int *const drugMassMgSelector, int *const drugMassUgSelector,
                     int *const patientMassSelector, int *const volumeDilutantSelector, int *const allowableFlowRateSelector, int *const show_dose, int *const dose_shown, int *const dropsPerMillilitre, unsigned long *button2PressedTime) {
  static unsigned long button2HeldTime = 0;
  static bool button2HeldFlag = false;

  if (*lastButton2State == LOW && button2State != LOW) {
    button2HeldFlag = false;
  }
  else if (*lastButton2State == LOW && button2State == LOW) {
    button2HeldTime = micros();
    if (button2HeldTime - *button2PressedTime >= BUTTON_HELD_TRIGGER_INCREMENT_TIME) {
      button2HeldFlag = true;
    }
  }

  if (*lastButton2State != LOW && button2State == LOW) {
    *button2PressedTime = micros();
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
      if (*drugMassUgSelector >= DRUG_UG_MASS_DEVIATION_UPPER_BOUND)
        *drugMassUgSelector = DRUG_UG_MASS_DEVIATION_LOWER_BOUND;

      else
        (*drugMassUgSelector)++;

    else if (Menu == 5 && *inputDrugMassUOMSelector == 1)
      if (*drugMassMgSelector >= DRUG_MG_MASS_DEVIATION_UPPER_BOUND)
        *drugMassMgSelector = DRUG_MG_MASS_DEVIATION_LOWER_BOUND;
      else
        (*drugMassMgSelector)++;

    else if (Menu == 6)
      if (*patientMassSelector >= PATIENT_MASS_UPPER_BOUND)
        *patientMassSelector = PATIENT_MASS_LOWER_BOUND;
      else
        (*patientMassSelector)++;

    else if (Menu == 7)
      if (*volumeDilutantSelector >= VOLUME_DILUTANT_UPPER_BOUND)
        *volumeDilutantSelector = VOLUME_DILUTANT_LOWER_BOUND;
      else
        (*volumeDilutantSelector)++;

    else if (Menu == 1)
      if (*allowableFlowRateSelector >= ALARM_DEVIATION_UPPER_BOUND)
        *allowableFlowRateSelector = ALARM_DEVIATION_LOWER_BOUND;
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

void evaluateButton3(const int button3State, int *lastButton3State, const int Menu, int *const dropsPerMillilitreSelector, int *const inputDrugMassUOMSelector, int *const drugMassMgSelector, int *const drugMassUgSelector,
                     int *const patientMassSelector, int *const volumeDilutantSelector, int *const allowableFlowRateSelector, int *const show_dose, int *const dose_shown, int *const dropsPerMillilitre) {
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

    else if (Menu == 5 && *inputDrugMassUOMSelector == 1)
      if (*drugMassUgSelector == DRUG_UG_MASS_DEVIATION_LOWER_BOUND)
        *drugMassUgSelector = DRUG_UG_MASS_DEVIATION_UPPER_BOUND;
      else
        (*drugMassUgSelector)--;
    else if (Menu == 5 && *inputDrugMassUOMSelector == 1)
      if (*drugMassMgSelector == DRUG_MG_MASS_DEVIATION_LOWER_BOUND)
        *drugMassMgSelector = DRUG_MG_MASS_DEVIATION_UPPER_BOUND;
      else
        (*drugMassMgSelector)--;

    else if (Menu == 6)
      if (*patientMassSelector == PATIENT_MASS_LOWER_BOUND)
        *patientMassSelector = PATIENT_MASS_UPPER_BOUND;
      else
        (*patientMassSelector)--;

    else if (Menu == 7)
      if (*volumeDilutantSelector == VOLUME_DILUTANT_LOWER_BOUND)
        *volumeDilutantSelector = VOLUME_DILUTANT_UPPER_BOUND;
      else
        (*volumeDilutantSelector)--;

    else if (Menu == 1)
      if (*allowableFlowRateSelector == 1)
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

void evaluateBuzzer(const bool BuzzerState, const double DisplayedFlowRate, const float lower_sound_thresh, const float upper_sound_thresh, const int BuzzerPin) {
  if (BuzzerState && (DisplayedFlowRate < lower_sound_thresh  || DisplayedFlowRate > upper_sound_thresh)) {
    digitalWrite(BuzzerPin, HIGH );
  }
  else {
    digitalWrite(BuzzerPin, LOW );
  }
}
