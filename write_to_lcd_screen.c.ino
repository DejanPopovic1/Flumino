
void printToScreen(const int Menu, const int dropsPerMillilitreSelector, const int allowableFlowRateSelector, const int show_dose, const int dose_shown, const int inputDrugMassUOMSelector, const int drugMassUgSelector,
                   const int drugMassMgSelector, const int patientMassSelector, const int volumeDilutantSelector, const double DisplayedFlowRate, const int BuzzerState, const double lower_sound_thresh,
                   const double upper_sound_thresh, const double lower_drugsound_thresh, const double upper_drugsound_thresh) {

  switch (Menu) {
    case drops_per_millilitre_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("    Drops/ml    ");
      display.println("");
      display.setTextColor(BLACK, WHITE);
      if (dropsPerMillilitreSelector == 0)
      {
        display.setTextColor(WHITE, BLACK);
        display.println("1) 10 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else
        display.println("1) 10 drops/ml");
      display.println("");
      if (dropsPerMillilitreSelector == 1)
      {
        display.setTextColor(WHITE, BLACK);
        display.println("2) 20 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else
        display.println("2) 20 drops/ml");
      display.println("");
      if (dropsPerMillilitreSelector == 2)
      {
        display.setTextColor(WHITE, BLACK);
        display.println("3) 60 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else
        display.println("3) 60 drops/ml");
      display.println("");
      display.refresh();
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
      display.print(allowableFlowRateSelector);
      display.refresh();
      break;

    case input_dosage_y_n_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println(" Input Dosage?  ");
      display.setTextColor(BLACK);
      display.print(" ");
      if (show_dose == false) {
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

      if (show_dose == true) {
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
      else
        ;
      display.refresh();
      break;

    case display_dosage_UOM_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println(" Input Dosage?  ");
      display.setTextColor(BLACK);
      display.print(" ");
      if (show_dose == false) {
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

      if (show_dose == true) {
        display.setTextColor(WHITE, BLACK);
        display.println(" Display Dosage Units of Measure");
        display.setTextColor(BLACK, WHITE);
        display.print(" ");

        if (dose_shown == 0) {
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

        else if (dose_shown == 1) {
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

        else if (dose_shown == 2) {
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
      else
        ;
      display.refresh();
      break;

    case input_drug_mass_UOM_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Input Drug Mass Units of Measure");
      display.setTextColor(BLACK);
      if (inputDrugMassUOMSelector == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else
        display.println("1) ug");
      display.println("");
      if (inputDrugMassUOMSelector == 1) {
        display.setTextColor(WHITE, BLACK);
        display.println("2) mg");
        display.setTextColor(BLACK);
      }
      else
        display.println("2) mg");
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("    Drug Mass   ");
      display.setTextColor(BLACK);
      display.print("   ");
      display.setTextSize(2);
      if (inputDrugMassUOMSelector == 0)
        display.println(drugMassUgSelector);
      else
        display.println(drugMassMgSelector);
      display.refresh();
      break;

    case drug_mass_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Input Drug Mass Units of Measure");
      display.setTextColor(BLACK);
      if (inputDrugMassUOMSelector == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else
        display.println("1) ug");
      display.println("");
      if (inputDrugMassUOMSelector == 1) {
        display.setTextColor(WHITE, BLACK);
        display.println("2) mg");
        display.setTextColor(BLACK);
      }
      else
        display.println("2) mg");
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("    Drug Mass   ");
      display.setTextColor(BLACK);
      display.print("   ");
      display.setTextSize(2);
      display.setTextColor(WHITE, BLACK);
      if (inputDrugMassUOMSelector == 0)
      {
        display.println(drugMassUgSelector);
      }
      else
        display.println(drugMassMgSelector);
      display.setTextColor(BLACK);
      display.refresh();
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
      display.println(patientMassSelector);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.println(volumeDilutantSelector);
      display.println("");
      display.refresh();
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
      display.println(patientMassSelector);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK);
      display.println(volumeDilutantSelector);
      display.println("");
      display.refresh();
      break;
    case flow_rate_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Saline Drip Rate     (ml/hr)    ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.println(DisplayedFlowRate);
      display.setTextSize(1);

      if (BuzzerState == 1) {
        display.print(lower_sound_thresh);
        display.print("-");
        display.print(upper_sound_thresh);
      }
      display.println("");
      display.setTextColor(WHITE, BLACK);

      if (show_dose == true) {
        if (dose_shown == 0) {
          display.print(" Drug Drip Rate ");
          display.println("   (ng/kg/min)  ");
        }
        else if (dose_shown == 1) {
          display.print(" Drug Drip Rate ");
          display.println("   (ug/kg/min)  ");
        }
        else if (dose_shown == 2) {
          display.print(" Drug Drip Rate ");
          display.println("   (mg/kg/min)  ");
        }
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.println(DrugFlowRate);
        display.setTextSize(1);
        if (BuzzerState == 1) {
          display.print(lower_drugsound_thresh);
          display.print("-");
          display.print(upper_drugsound_thresh);
        }
      }
      else
        ;
      display.refresh();
      break;
    default:
      break;
  }
}
