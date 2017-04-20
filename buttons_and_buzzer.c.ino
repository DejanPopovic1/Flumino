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

bool evaluateSensor(struct MachineState *s) {
  if (s->previoiusSensorReading == HIGH && s->sensorReading == LOW) {
    return true;
  }
  else {
    return false;
  }
}

void evaluateFlowRate(bool isDropPassing, struct MachineState *s) {
  if (isDropPassing == true) {
    s->newFlowRate = flowRate(s);
  }
  if (s->currentTime >= s->previousTime + s->period) {
    s->newFlowRate =  decayedFlowRate(s->previousTime, s->currentTime, s->dropsPerMillilitre, s->period);
  }
  if (s->currentTime % DISPLAY_PERIOD <= MAXIMUM_CYCLE_TIME) {
    s->DisplayedFlowRate = s->newFlowRate;
  }
}

void evaluateButton1(struct MachineState *s) {
  if (s->lastButton1State != LOW && s->button1State == LOW) {
    if (s->button1PushCounter == 8)
    {
      s->button1PushCounter = 0;
    }
    else if (s->button1PushCounter == 2 && s->show_dose == false)
    {
      s->button1PushCounter = 8;
    }
    else
    {
      (s->button1PushCounter)++;
    }
  }
}

void evaluateButton2(struct MachineState *s) {
  static unsigned long button2HeldTime = 0;
  static bool button2HeldFlag = false;

  if (s->lastButton2State == LOW && s->button2State != LOW) {
    button2HeldFlag = false;
  }
  else if (s->lastButton2State == LOW && s->button2State == LOW) {
    button2HeldTime = micros();
    if (button2HeldTime - s->button2PressedTime >= BUTTON_HELD_TRIGGER_INCREMENT_TIME) {
      button2HeldFlag = true;
    }
  }

  if (s->lastButton2State != LOW && s->button2State == LOW) {
    s->button2PressedTime = micros();
    if (s->Menu == 0) {
      if (s->dropsPerMillilitreSelector >= 2)
        s->dropsPerMillilitreSelector = 0;
      else
        (s->dropsPerMillilitreSelector)++;
      switch (s->dropsPerMillilitreSelector) {
        case 0:
          s->dropsPerMillilitre = 10;
          break;
        case 1:
          s->dropsPerMillilitre = 20;
          break;
        case 2:
          s->dropsPerMillilitre = 60;
          break;
      }
    }
    else if (s->Menu == 4)
      if (s->inputDrugMassUOMSelector >= 1)
        s->inputDrugMassUOMSelector = 0;
      else
        (s->inputDrugMassUOMSelector)++;

    else if (s->Menu == 5 && s->inputDrugMassUOMSelector == 0)
      if (s->drugMassUgSelector >= DRUG_UG_MASS_DEVIATION_UPPER_BOUND)
        s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_LOWER_BOUND;

      else
        (s->drugMassUgSelector)++;

    else if (s->Menu == 5 && s->inputDrugMassUOMSelector == 1)
      if (s->drugMassMgSelector >= DRUG_MG_MASS_DEVIATION_UPPER_BOUND)
        s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_LOWER_BOUND;
      else
        (s->drugMassMgSelector)++;

    else if (s->Menu == 6)
      if (s->patientMassSelector >= PATIENT_MASS_UPPER_BOUND)
        s->patientMassSelector = PATIENT_MASS_LOWER_BOUND;
      else
        (s->patientMassSelector)++;

    else if (s->Menu == 7)
      if (s->volumeDilutantSelector >= VOLUME_DILUTANT_UPPER_BOUND)
        s->volumeDilutantSelector = VOLUME_DILUTANT_LOWER_BOUND;
      else
        (s->volumeDilutantSelector)++;

    else if (s->Menu == 1)
      if (s->allowableFlowRateSelector >= ALARM_DEVIATION_UPPER_BOUND)
        s->allowableFlowRateSelector = ALARM_DEVIATION_LOWER_BOUND;
      else
        (s->allowableFlowRateSelector)++;

    else if (s->Menu == 2)
      if (s->show_dose == true)
        s->show_dose = false;
      else
        s->show_dose = true;

    else if (s->Menu == 3)
      if (s->dose_shown >= 2)
        s->dose_shown = 0;
      else
        (s->dose_shown)++;
    delay(1);
  }
}

void evaluateButton3(struct MachineState *s) {
  if (s->lastButton3State != LOW && s->button3State == LOW) {
    if (s->Menu == 0) {
      if (s->dropsPerMillilitreSelector == 0)
        s->dropsPerMillilitreSelector = 2;
      else
        (s->dropsPerMillilitreSelector)--;
      switch (s->dropsPerMillilitreSelector) {
        case 0:
          s->dropsPerMillilitre = 10;
          break;
        case 1:
          s->dropsPerMillilitre = 20;
          break;
        case 2:
          s->dropsPerMillilitre = 60;
          break;
      }
    }
    else if (s->Menu == 4)
      if (s->inputDrugMassUOMSelector == 0)
        s->inputDrugMassUOMSelector = 1;
      else
        (s->inputDrugMassUOMSelector)--;

    else if (s->Menu == 5 && s->inputDrugMassUOMSelector == 0)
      if (s->drugMassUgSelector == DRUG_UG_MASS_DEVIATION_LOWER_BOUND)
        s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_UPPER_BOUND;
      else
        (s->drugMassUgSelector)--;
    else if (s->Menu == 5 && s->inputDrugMassUOMSelector == 1)
      if (s->drugMassMgSelector == DRUG_MG_MASS_DEVIATION_LOWER_BOUND)
        s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_UPPER_BOUND;
      else
        (s->drugMassMgSelector)--;

    else if (s->Menu == 6)
      if (s->patientMassSelector == PATIENT_MASS_LOWER_BOUND)
        s->patientMassSelector = PATIENT_MASS_UPPER_BOUND;
      else
        (s->patientMassSelector)--;

    else if (s->Menu == 7)
      if (s->volumeDilutantSelector == VOLUME_DILUTANT_LOWER_BOUND)
        s->volumeDilutantSelector = VOLUME_DILUTANT_UPPER_BOUND;
      else
        (s->volumeDilutantSelector)--;

    else if (s->Menu == 1)
      if (s->allowableFlowRateSelector == 1)
        s->allowableFlowRateSelector = 100;
      else
        (s->allowableFlowRateSelector)--;

    else if (s->Menu == 2)
      if (s->show_dose == true)
        s->show_dose = false;
      else
        s->show_dose = true;

    else if (s->Menu == 3)
      if (s->dose_shown == 0)
        s->dose_shown = 2;
      else
        (s->dose_shown)--;
    delay(1);
  }
}

void evaluateButton4(struct MachineState *s) {
  if (s->lastButton4State != LOW && s->button4State == LOW && s->Menu == flow_rate_page) {
    s->BuzzerState = !(s->BuzzerState);
    s->lower_sound_thresh = s->newFlowRate * (1 - (float)s->allowableFlowRateSelector / 100);
    s->upper_sound_thresh = s->newFlowRate * (1 + (float)s->allowableFlowRateSelector / 100);
    s->lower_drugsound_thresh = s->DrugFlowRate * (1 - (float)s->allowableFlowRateSelector / 100);
    s->upper_drugsound_thresh = s->DrugFlowRate * (1 + (float)s->allowableFlowRateSelector / 100);
  }
}

void evaluateBuzzer(struct MachineState *s) {
  if (s->BuzzerState && (s->DisplayedFlowRate < s->lower_sound_thresh  || s->DisplayedFlowRate > s->upper_sound_thresh)) {
    digitalWrite(BuzzerPin, HIGH );
  }
  else {
    digitalWrite(BuzzerPin, LOW );
  }
}
