void printToScreen(struct MachineState *s) {
  bool screenRefreshEvent = s->button1Status == BUTTON_PRESSED || s->button2Status == BUTTON_PRESSED || s->button2Status == BUTTON_INCREMENTED || s->button3Status == BUTTON_PRESSED || 
                            s->button3Status == BUTTON_INCREMENTED || s->button4Status == BUTTON_PRESSED || s->sensorStatus == DROP_PASSING;
  if(!screenRefreshEvent) {
    return;
  }
  display.clearDisplay();
  switch (s -> Menu) {
    case drops_per_millilitre_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("    Drops/ml    ");
      display.println("");
      display.setTextColor(BLACK, WHITE);
      if (s -> dropsPerMillilitreSelector == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) 10 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else {
        display.println("1) 10 drops/ml");
      }
      display.println("");
      if (s -> dropsPerMillilitreSelector == 1) {
        display.setTextColor(WHITE, BLACK);
        display.println("2) 20 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else {
        display.println("2) 20 drops/ml");
      }
      display.println("");
      if (s -> dropsPerMillilitreSelector == 2) {
        display.setTextColor(WHITE, BLACK);
        display.println("3) 60 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else {
        display.println("3) 60 drops/ml");
      }
      display.println("");
      break;

    case allowable_flow_rate_deviation_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Allowable Flow  rate Deviation  without activat-ing alarm       (+/- Percentage)");
      display.setTextColor(BLACK);
      display.print(" ");
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setTextSize(2);
      display.print(s -> allowableFlowRateSelector);
      break;

    case input_dosage_y_n_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println(" Input Dosage?  ");
      display.setTextColor(BLACK);
      display.print(" ");
      if (s -> show_dose == false) {
        display.print("Yes");
        display.print(" / ");
        display.setTextColor(WHITE, BLACK);
        display.println("No");
        display.setTextColor(BLACK);
        display.println(" ");
      }
      else {
        display.setTextColor(WHITE, BLACK);
        display.print("Yes");
        display.setTextColor(BLACK);
        display.print(" / ");
        display.println(" No ");
        display.setTextColor(BLACK);
        display.println(" ");
      }
      if (s -> show_dose == true) {
        display.setTextColor(WHITE, BLACK);
        display.println(" Display Dosage Units of Measure");
        display.setTextColor(BLACK, WHITE);
        display.print(" ");
        display.println("ng/kg/min");
        display.println(" ");
        display.print(" ");
        display.println("ug/kg/min");
        display.println(" ");
        display.print(" ");
        display.println("mg/kg/min");
      }
      break;

    case display_dosage_UOM_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println(" Input Dosage?  ");
      display.setTextColor(BLACK);
      display.print(" ");
      if (s -> show_dose == false) {
        display.print("Yes");
        display.print(" / ");
        display.setTextColor(WHITE, BLACK);
        display.println("No");
        display.setTextColor(BLACK);
        display.println(" ");
      }
      else {
        display.setTextColor(WHITE, BLACK);
        display.print("Yes");
        display.setTextColor(BLACK);
        display.print(" / ");
        display.println(" No ");
        display.setTextColor(BLACK);
        display.println(" ");
      }
      if (s -> show_dose == true) {
        display.setTextColor(WHITE, BLACK);
        display.println(" Display Dosage Units of Measure");
        display.setTextColor(BLACK, WHITE);
        display.print(" ");
        if (s -> dose_shown == 0) {
          display.setTextColor(WHITE, BLACK);
          display.println("ng/kg/min");
          display.setTextColor(BLACK, WHITE);
          display.println(" ");
          display.print(" ");
          display.println("ug/kg/min");
          display.println(" ");
          display.print(" ");
          display.println("mg/kg/min");
        }
        else if (s -> dose_shown == 1) {
          display.println("ng/kg/min");
          display.println(" ");
          display.print(" ");
          display.setTextColor(WHITE, BLACK);
          display.println("ug/kg/min");
          display.setTextColor(BLACK, WHITE);
          display.println(" ");
          display.print(" ");
          display.println("mg/kg/min");
        }
        else if (s -> dose_shown == 2) {
          display.println("ng/kg/min");
          display.println(" ");
          display.print(" ");
          display.println("ug/kg/min");
          display.println(" ");
          display.print(" ");
          display.setTextColor(WHITE, BLACK);
          display.println("mg/kg/min");
          display.setTextColor(BLACK, WHITE);
        }
      }
      break;

    case input_drug_mass_UOM_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Input Drug Mass Units of Measure");
      display.setTextColor(BLACK);
      if (s -> inputDrugMassUOMSelector == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else
        display.println("1) ug");
      display.println("");
      if (s -> inputDrugMassUOMSelector == 1) {
        display.setTextColor(WHITE, BLACK);
        display.println("2) mg");
        display.setTextColor(BLACK);
      }
      else {
        display.println("2) mg");
      }
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("    Drug Mass   ");
      display.setTextColor(BLACK);
      display.print("   ");
      display.setTextSize(2);
      if (s -> inputDrugMassUOMSelector == 0) {
        display.println(s -> drugMassUgSelector);
      }
      else {
        display.println(s -> drugMassMgSelector);
      }
      break;

    case drug_mass_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Input Drug Mass Units of Measure");
      display.setTextColor(BLACK);
      if (s -> inputDrugMassUOMSelector == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else {
        display.println("1) ug");
      }
      display.println("");
      if (s -> inputDrugMassUOMSelector == 1) {
        display.setTextColor(WHITE, BLACK);
        display.println("2) mg");
        display.setTextColor(BLACK);
      }
      else {
        display.println("2) mg");
      }
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("    Drug Mass   ");
      display.setTextColor(BLACK);
      display.print("   ");
      display.setTextSize(2);
      display.setTextColor(WHITE, BLACK);
      if (s -> inputDrugMassUOMSelector == 0) {
        display.println(s -> drugMassUgSelector);
      }
      else {
        display.println(s -> drugMassMgSelector);
      }
      display.setTextColor(BLACK);
      break;
    case patient_mass_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.print("  Patient Mass        (kg)      ");
      display.setTextColor(BLACK);
      display.println(" ");
      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK);
      display.println(s -> patientMassSelector);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.println(s -> volumeDilutantSelector);
      display.println("");
      break;
    case volume_dilutent_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.print("  Patient Mass        (kg)      ");
      display.setTextColor(BLACK);
      display.println(" ");
      display.setTextSize(2);
      display.print(" ");
      display.println(s -> patientMassSelector);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK);
      display.println(s -> volumeDilutantSelector);
      display.println("");
      break;
    case flow_rate_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Saline Drip Rate     (ml/hr)    ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.println(s -> DisplayedFlowRate);
      display.setTextSize(1);
      if (s -> BuzzerState == 1) {
        display.print(s -> lower_sound_thresh);
        display.print("-");
        display.print(s -> upper_sound_thresh);
      }
      display.println("");
      display.setTextColor(WHITE, BLACK);
      if (s -> show_dose == true) {
        if (s -> dose_shown == 0) {
          display.print(" Drug Drip Rate ");
          display.println("   (ng/kg/min)  ");
        }
        else if (s -> dose_shown == 1) {
          display.print(" Drug Drip Rate ");
          display.println("   (ug/kg/min)  ");
        }
        else if (s -> dose_shown == 2) {
          display.print(" Drug Drip Rate ");
          display.println("   (mg/kg/min)  ");
        }
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.println(s -> DrugFlowRate);
        display.setTextSize(1);
        if (s -> BuzzerState == 1) {
          display.print(s -> lower_drugsound_thresh);
          display.print("-");
          display.print(s -> upper_drugsound_thresh);
        }
      }
      break;
  }
    display.refresh();
    //delay(10);
}
