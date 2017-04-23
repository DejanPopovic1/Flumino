#define MAXIMUM_CYCLE_TIME                  20000
#define DISPLAY_PERIOD                      1000000

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
    s->previousTime = s->currentTime;
  }
  s->DisplayedFlowRate = s->flowRate;//ADDED TOGETHER WITH COMMENTED OUT SECTIONS
//  if (s->currentTime >= s->previousTime + s->period) {
//    s->flowRate =  decayedFlowRate(s->previousTime, s->currentTime, s->dropsPerMillilitre, s->period);
//  }
//  if (s->currentTime % DISPLAY_PERIOD <= MAXIMUM_CYCLE_TIME) {
//    s->DisplayedFlowRate = s->flowRate;
//  }
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
      (s->dropsPerMillilitreSelector >= 2) ? s->dropsPerMillilitreSelector = 0 : (s->dropsPerMillilitreSelector)++;
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
      s->inputDrugMassUOMSelector >= 1 ? s->inputDrugMassUOMSelector = 0 : s->inputDrugMassUOMSelector = (*incrementFunctionPointer)(s->inputDrugMassUOMSelector, input_drug_mass_UOM_page);
    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 0)
      s->drugMassUgSelector >= DRUG_UG_MASS_DEVIATION_UPPER_BOUND ? s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_LOWER_BOUND : s->drugMassUgSelector = (*incrementFunctionPointer)(s->drugMassUgSelector, drug_mass_page);
    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 1)
      s->drugMassMgSelector >= DRUG_MG_MASS_DEVIATION_UPPER_BOUND ? s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_LOWER_BOUND : s->drugMassMgSelector = (*incrementFunctionPointer)(s->drugMassMgSelector, drug_mass_page);
    else if (s->Menu == patient_mass_page)
      s->patientMassSelector >= PATIENT_MASS_UPPER_BOUND ? s->patientMassSelector = PATIENT_MASS_LOWER_BOUND : s->patientMassSelector = (*incrementFunctionPointer)(s->patientMassSelector, patient_mass_page);
    else if (s->Menu == volume_dilutent_page)
      s->volumeDilutantSelector >= VOLUME_DILUTANT_UPPER_BOUND ? s->volumeDilutantSelector = VOLUME_DILUTANT_LOWER_BOUND : s->volumeDilutantSelector = (*incrementFunctionPointer)(s->volumeDilutantSelector, volume_dilutent_page);
    else if (s->Menu == allowable_flow_rate_deviation_page)
      s->allowableFlowRateSelector >= ALARM_DEVIATION_UPPER_BOUND ? s->allowableFlowRateSelector = ALARM_DEVIATION_LOWER_BOUND : s->allowableFlowRateSelector = (*incrementFunctionPointer)(s->allowableFlowRateSelector, allowable_flow_rate_deviation_page);
    else if (s->Menu == input_dosage_y_n_page)
      s->show_dose = !(s->show_dose);
    else if (s->Menu == display_dosage_UOM_page)
      s->dose_shown >= 2 ? s->dose_shown = 0 : s->dose_shown = (*incrementFunctionPointer)(s->dose_shown, display_dosage_UOM_page);
  }
}

void evaluateButton3(struct MachineState *s, int (*incrementFunctionPointer)(int, int)) {
  if (s->button3Status == BUTTON_PRESSED || s->button3Status == BUTTON_INCREMENTED) {
    if (s->Menu == drops_per_millilitre_page) {
      (s->dropsPerMillilitreSelector <= 0) ? s->dropsPerMillilitreSelector = 2 : (s->dropsPerMillilitreSelector)--;
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
      s->inputDrugMassUOMSelector <= 0 ? s->inputDrugMassUOMSelector = 1 : s->inputDrugMassUOMSelector = (*incrementFunctionPointer)(s->inputDrugMassUOMSelector, input_drug_mass_UOM_page);
    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 0)
      s->drugMassUgSelector <= DRUG_UG_MASS_DEVIATION_LOWER_BOUND ? s->drugMassUgSelector = DRUG_UG_MASS_DEVIATION_UPPER_BOUND : s->drugMassUgSelector = (*incrementFunctionPointer)(s->drugMassUgSelector, drug_mass_page);
    else if (s->Menu == drug_mass_page && s->inputDrugMassUOMSelector == 1)
      s->drugMassMgSelector <= DRUG_MG_MASS_DEVIATION_LOWER_BOUND ? s->drugMassMgSelector = DRUG_MG_MASS_DEVIATION_UPPER_BOUND : s->drugMassMgSelector = (*incrementFunctionPointer)(s->drugMassMgSelector, drug_mass_page);
    else if (s->Menu == patient_mass_page)
      s->patientMassSelector <= PATIENT_MASS_LOWER_BOUND ? s->patientMassSelector = PATIENT_MASS_UPPER_BOUND : s->patientMassSelector = (*incrementFunctionPointer)(s->patientMassSelector, patient_mass_page);
    else if (s->Menu == volume_dilutent_page)
      s->volumeDilutantSelector <= VOLUME_DILUTANT_LOWER_BOUND ? s->volumeDilutantSelector = VOLUME_DILUTANT_UPPER_BOUND : s->volumeDilutantSelector = (*incrementFunctionPointer)(s->volumeDilutantSelector, volume_dilutent_page);
    else if (s->Menu == allowable_flow_rate_deviation_page)
      s->allowableFlowRateSelector <= ALARM_DEVIATION_LOWER_BOUND ? s->allowableFlowRateSelector = ALARM_DEVIATION_UPPER_BOUND : s->allowableFlowRateSelector = (*incrementFunctionPointer)(s->allowableFlowRateSelector, allowable_flow_rate_deviation_page);
    else if (s->Menu == input_dosage_y_n_page)
      s->show_dose = !(s->show_dose);
    else if (s->Menu == display_dosage_UOM_page)
      s->dose_shown <= 0 ? s->dose_shown = 2 : s->dose_shown = (*incrementFunctionPointer)(s->dose_shown, display_dosage_UOM_page);
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

int smallIncrement(int value, int dummyVariable) {
  return ++value;
}

int smallDecrement(int value, int dummyVariable) {
  return --value;
}

int largeIncrement(int value, int valueToIncrement) {
  switch(valueToIncrement) {
  case 1:
    value += BUTTON_HELD_INCREMENT_ALARM_DEVIATION;
    break;
  case 5:
    value += BUTTON_HELD_INCREMENT_DRUG_MASS_UG;
    break;
  case 6:
    value += BUTTON_HELD_INCREMENT_PATIENT_MASS;
    break;
  case 7:
    value += BUTTON_HELD_INCREMENT_VOLUME_DILUTANT;
    break;
  default:
    value++;  
  }
  return value;
}

int largeDecrement(int value, int valueToDecrement) {
  switch(valueToDecrement) {
  case 1:
    value -= BUTTON_HELD_INCREMENT_ALARM_DEVIATION;
    break;
  case 5:
    value -= BUTTON_HELD_INCREMENT_DRUG_MASS_UG;
    break;
  case 6:
    value -= BUTTON_HELD_INCREMENT_PATIENT_MASS;
    break;
  case 7:
    value -= BUTTON_HELD_INCREMENT_VOLUME_DILUTANT;
    break;
  default:
    value--;  
  }
  return value;
}
