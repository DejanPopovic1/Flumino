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

#define TEN_DROPS_PER_MILLILITRE            10
#define TWENTY_DROPS_PER_MILLILITRE         20
#define SIXTY_DROPS_PER_MILLILITRE         30

#define BUTTON_HELD_INCREMENT_ALARM_DEVIATION 1
#define BUTTON_HELD_INCREMENT_DRUG_MASS_UG 10
#define BUTTON_HELD_INCREMENT_DRUG_MASS_MG 10
#define BUTTON_HELD_INCREMENT_PATIENT_MASS 5
#define BUTTON_HELD_INCREMENT_VOLUME_DILUTANT 50

void evaluateFlowRate(struct MachineState *s) {
  if (s->sensorStatus == DROP_PASSING) {
    s->flowRate = flowRate(s);
  }
  if (s->currentTime >= s->previousTime + s->period) {
    s->flowRate =  decayedFlowRate(s->previousTime, s->currentTime, s->dropsPerMillilitre, s->period);
  }
  if (s->currentTime % DISPLAY_PERIOD <= MAXIMUM_CYCLE_TIME) {
    s->DisplayedFlowRate = s->flowRate;
  }
}

void evaluateButton1(struct MachineState *s) {
  if (s->lastButton1State != LOW && s->button1State == LOW) {
    if (s->Menu == flow_rate_page) {
      s->Menu = drops_per_millilitre_page;
    }
    else if (s->Menu == input_dosage_y_n_page && s->show_dose == false) {
      s->Menu = flow_rate_page;
    }
    else {
      (s->Menu)++;
    }
  }
}

void evaluateButton2(struct MachineState *s, int (*incrementFunctionPointer)(int, int)) {
  if (s->button2Status == BUTTON_PRESSED || s->button2Status == BUTTON_INCREMENTED) {
    if (s->Menu == drops_per_millilitre_page) {
      if (s->dropsPerMillilitreSelector >= 2)
        s->dropsPerMillilitreSelector = 0;
      else
        (s->dropsPerMillilitreSelector)++;
      switch (s->dropsPerMillilitreSelector) {
        case 0:
          s->dropsPerMillilitre = TEN_DROPS_PER_MILLILITRE;
          break;
        case 1:
          s->dropsPerMillilitre = TWENTY_DROPS_PER_MILLILITRE;
          break;
        case 2:
          s->dropsPerMillilitre = SIXTY_DROPS_PER_MILLILITRE;
          break;
      }
    }
    else if (s->Menu == input_drug_mass_UOM_page)
      if (s->inputDrugMassUOMSelector >= 1)
        s->inputDrugMassUOMSelector = 0;
      else
        (s->inputDrugMassUOMSelector)++;

    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 0)
      if (s->drugMassUgSelector >= DRUG_UG_MASS_DEVIATION_UPPER_BOUND)
        s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_LOWER_BOUND;

      else
        (s->drugMassUgSelector)++;

    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 1)
      if (s->drugMassMgSelector >= DRUG_MG_MASS_DEVIATION_UPPER_BOUND)
        s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_LOWER_BOUND;
      else
        (s->drugMassMgSelector)++;

    else if (s->Menu == patient_mass_page)
      if (s->patientMassSelector >= PATIENT_MASS_UPPER_BOUND)
        s->patientMassSelector = PATIENT_MASS_LOWER_BOUND;
      else
        (s->patientMassSelector)++;

    else if (s->Menu == volume_dilutent_page)
      if (s->volumeDilutantSelector >= VOLUME_DILUTANT_UPPER_BOUND)
        s->volumeDilutantSelector = VOLUME_DILUTANT_LOWER_BOUND;
      else
        s->volumeDilutantSelector = (*incrementFunctionPointer)(s->volumeDilutantSelector, 7);

    else if (s->Menu == allowable_flow_rate_deviation_page)
      if (s->allowableFlowRateSelector >= ALARM_DEVIATION_UPPER_BOUND)
        s->allowableFlowRateSelector = ALARM_DEVIATION_LOWER_BOUND;
      else
        (s->allowableFlowRateSelector)++;

    else if (s->Menu == input_dosage_y_n_page)
      if (s->show_dose == true)
        s->show_dose = false;
      else
        s->show_dose = true;

    else if (s->Menu == display_dosage_UOM_page)
      if (s->dose_shown >= 2)
        s->dose_shown = 0;
      else
        (s->dose_shown)++;
    delay(1);
  }
}

void evaluateButton3(struct MachineState *s) {
  if (s->lastButton3State != LOW && s->button3State == LOW) {
    if (s->Menu == drops_per_millilitre_page) {
      if (s->dropsPerMillilitreSelector == 0)
        s->dropsPerMillilitreSelector = 2;
      else
        (s->dropsPerMillilitreSelector)--;
      switch (s->dropsPerMillilitreSelector) {
        case 0:
          s->dropsPerMillilitre = TEN_DROPS_PER_MILLILITRE;
          break;
        case 1:
          s->dropsPerMillilitre = TWENTY_DROPS_PER_MILLILITRE;
          break;
        case 2:
          s->dropsPerMillilitre = SIXTY_DROPS_PER_MILLILITRE;
          break;
      }
    }
    else if (s->Menu == input_drug_mass_UOM_page)
      if (s->inputDrugMassUOMSelector == 0)
        s->inputDrugMassUOMSelector = 1;
      else
        (s->inputDrugMassUOMSelector)--;

    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 0)
      if (s->drugMassUgSelector == DRUG_UG_MASS_DEVIATION_LOWER_BOUND)
        s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_UPPER_BOUND;
      else
        (s->drugMassUgSelector)--;
    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 1)
      if (s->drugMassMgSelector == DRUG_MG_MASS_DEVIATION_LOWER_BOUND)
        s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_UPPER_BOUND;
      else
        (s->drugMassMgSelector)--;

    else if (s->Menu == patient_mass_page)
      if (s->patientMassSelector == PATIENT_MASS_LOWER_BOUND)
        s->patientMassSelector = PATIENT_MASS_UPPER_BOUND;
      else
        (s->patientMassSelector)--;

    else if (s->Menu == volume_dilutent_page)
      if (s->volumeDilutantSelector == VOLUME_DILUTANT_LOWER_BOUND)
        s->volumeDilutantSelector = VOLUME_DILUTANT_UPPER_BOUND;
      else
        (s->volumeDilutantSelector)--;

    else if (s->Menu == allowable_flow_rate_deviation_page)
      if (s->allowableFlowRateSelector == 1)
        s->allowableFlowRateSelector = 100;
      else
        (s->allowableFlowRateSelector)--;

    else if (s->Menu == input_dosage_y_n_page)
      if (s->show_dose == true)
        s->show_dose = false;
      else
        s->show_dose = true;

    else if (s->Menu == display_dosage_UOM_page)
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
    s->lower_sound_thresh = s->flowRate * (1 - (float)s->allowableFlowRateSelector / 100);
    s->upper_sound_thresh = s->flowRate * (1 + (float)s->allowableFlowRateSelector / 100);
    s->lower_drugsound_thresh = s->DrugFlowRate * (1 - (float)s->allowableFlowRateSelector / 100);
    s->upper_drugsound_thresh = s->DrugFlowRate * (1 + (float)s->allowableFlowRateSelector / 100);
  }
}

void evaluateBuzzer(struct MachineState *s) {
  if (s->BuzzerState && (s->DisplayedFlowRate < s->lower_sound_thresh  || s->DisplayedFlowRate > s->upper_sound_thresh)) {
    digitalWrite(BuzzerPin, HIGH);
  }
  else {
    digitalWrite(BuzzerPin, LOW);
  }
}

int smallIncrement(int initialValue, int dummyVariable) {
  return ++initialValue;
}

int largeIncrement(int initialValue, int valueToIncrement) {
  switch(valueToIncrement) {
  case 1:
    initialValue += BUTTON_HELD_INCREMENT_ALARM_DEVIATION;
    break;
  case 5:
    initialValue += BUTTON_HELD_INCREMENT_DRUG_MASS_UG;
    break;
  case 6:
    initialValue += BUTTON_HELD_INCREMENT_PATIENT_MASS;
    break;
  case 7:
    initialValue += BUTTON_HELD_INCREMENT_VOLUME_DILUTANT;
    break;
  default:
    initialValue++;  
  }
  return initialValue;
}
