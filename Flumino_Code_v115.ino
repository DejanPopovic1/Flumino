#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <SPI.h>
#define SCK                      5
#define MOSI                     6
#define SS                       7
#define BLACK                    0
#define WHITE                    1
#define DISPLAY_PERIOD           2000000
#define MAXIMUM_CYCLE_TIME       20000
#define SENSOR_THRESHOLD 600
Adafruit_SharpMem display(SCK, MOSI, SS);  //For the SPI BUS

enum menu_page
{
  drops_per_millilitre_page,
  allowable_flow_rate_deviation_page,
  input_dosage_y_n_page,
  display_dosage_UOM_page,
  input_drug_mass_UOM_page,
  drug_mass_page,
  patient_mass_page,
  volume_dilutent_page,
  flow_rate_page
};

int Menu;
int previoiusSensorReading      = 0;     // previous state of the button
float FlowRate                  = 180;
float DisplayedFlowRate;
float DrugFlowRate              = 0;
unsigned long period            = 0;
int dropsPerMillilitre          = 20;
int show_dose                   = false;
int dose_shown                  = 1;
float lower_sound_thresh        = 0;
float upper_sound_thresh        = 0;
float lower_drugsound_thresh    = 0;
float upper_drugsound_thresh    = 0;
int sensorReading               = 0;
int IROutPin                    = 12;         // the number of the input pin
const int analogInPin           = A1;  // Analog input pin that the potentiometer is attached to
const int BuzzerPin             = 3;

//------Button-1-Code-----------------
const int Button1InPin          = A0;
int button1PushCounter          = 8;
int button1State                = 0;
int lastButton1State            = 0;
//------Button-2-Code-----------------
const int Button2InPin          = A2;
int button2PushCounter          = 1;
int button2State                = 0;
int lastButton2State            = 0;
//------Button-3-Code-----------------
const int Button3InPin          = A3;
int button3PushCounter          = 1;
int button3State                = 0;
int lastButton3State            = 0;
//------Button-4-Code-----------------
const int Button4InPin          = 2;
int button4PushCounter          = false;
int button4State                = 0;
int lastButton4State            = 1;

//--------Menu-Variable-Counters-------//
int Case0Count                  = 1;
int Case1Count                  = 0;
int Case2CountUg                = 100;
int Case2CountMg                = 1;
int Case3Count                  = 65;
int Case4Count                  = 1000;
int Case5Count                  = 12;

bool BuzzerState = false;
double newFlowRate = 180;
unsigned long previousTime = 0;
bool isDropPassing = false;

void setup()
{
  display.begin();
  Serial.begin(9600);
  digitalWrite(12, HIGH);
  pinMode(13, OUTPUT); //LED
  pinMode(3, OUTPUT);   //Buzzer
  pinMode(2, INPUT);   //Button 4
  pinMode(IROutPin, OUTPUT); //Infrared LED
}
unsigned long currentTime;

void loop()
{
  currentTime = micros();
  delayMicroseconds(1);

  if (analogRead(analogInPin) >= SENSOR_THRESHOLD)
  {
    sensorReading = HIGH;
  }
  else
  {
    sensorReading = LOW;
  }

  if (previoiusSensorReading == HIGH && sensorReading == LOW)
  {
    isDropPassing = true;
  }
  else
  {
    isDropPassing = false;
  }
  previoiusSensorReading = sensorReading;
  if (isDropPassing == true)
  {
    newFlowRate = flowRate(&previousTime, currentTime, dropsPerMillilitre, &newFlowRate, &period);
  }
  if (currentTime >= previousTime + period)
  {
    newFlowRate = decayedFlowRate(previousTime, currentTime, dropsPerMillilitre, period);
  }
  if (currentTime % DISPLAY_PERIOD <= MAXIMUM_CYCLE_TIME) {
    DisplayedFlowRate = newFlowRate;
  }

  button1State = analogRead(Button1InPin);
  button2State = analogRead(Button2InPin);
  button3State = analogRead(Button3InPin);
  button4State = digitalRead(Button4InPin);
  evaluateButton1(button1State, &lastButton1State, &button1PushCounter, show_dose);
  evaluateButton2(button2State, &lastButton2State, Menu, &Case0Count, &Case1Count, &Case2CountMg, &Case2CountUg, &Case3Count, &Case4Count, &Case5Count, &show_dose, &dose_shown);
  evaluateButton3(button3State, &lastButton3State, Menu, &Case0Count, &Case1Count, &Case2CountMg, &Case2CountUg, &Case3Count, &Case4Count, &Case5Count, &show_dose, &dose_shown);
  evaluateButton4(button4State, &lastButton4State, &BuzzerState, &lower_sound_thresh, &upper_sound_thresh, &lower_drugsound_thresh, &upper_drugsound_thresh, newFlowRate, DrugFlowRate, Case5Count);

  if (BuzzerState){
    if ((newFlowRate < lower_sound_thresh  || newFlowRate > upper_sound_thresh)) {
      digitalWrite(BuzzerPin, HIGH );
    }
    else{
      digitalWrite(BuzzerPin, LOW );
    }
  }
  else{
    digitalWrite(BuzzerPin, LOW );
  }

  switch (Case0Count){
    case 0:
      dropsPerMillilitre = 10;
      break;
    case 1:
      dropsPerMillilitre = 20;
      break;
    case 2:
      dropsPerMillilitre = 60;
      break;
  }

  if (Case1Count == 0 && dose_shown == 0)
    DrugFlowRate = (Case2CountUg * FlowRate) / (Case4Count * 60 * Case3Count) * 1000 ;
  else if (Case1Count == 0 && dose_shown == 1)
    DrugFlowRate = (Case2CountUg * FlowRate) / (Case4Count * 60 * Case3Count);
  else if (Case1Count == 0 && dose_shown == 2)
    DrugFlowRate = (Case2CountUg * FlowRate) / (Case4Count * 60 * Case3Count) / 1000;
  else if (Case1Count == 1 && dose_shown == 0)
    DrugFlowRate = (Case2CountMg * FlowRate) / (Case4Count * 60 * Case3Count) * 1000000;
  else if (Case1Count == 1 && dose_shown == 1)
    DrugFlowRate = (Case2CountMg * FlowRate) / (Case4Count * 60 * Case3Count) * 1000;
  else if (Case1Count == 1 && dose_shown == 2)
    DrugFlowRate = (Case2CountMg * FlowRate) / (Case4Count * 60 * Case3Count);

  //-S-3.1-----Setup-LCD----------//
  display.clearDisplay();
  
  Menu = button1PushCounter;
  //-S-3.2-----Write-To-LCD----------//
  switch (Menu) {
    case drops_per_millilitre_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setCursor(0, 0);
      display.println("    Drops/ml    ");
      display.println("");
      display.setTextColor(BLACK, WHITE);
      if (Case0Count == 0)
      {
        display.setTextColor(WHITE, BLACK);
        display.println("1) 10 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else
        display.println("1) 10 drops/ml");
      display.println("");
      if (Case0Count == 1)
      {
        display.setTextColor(WHITE, BLACK);
        display.println("2) 20 drops/ml");
        display.setTextColor(BLACK, WHITE);
      }
      else
        display.println("2) 20 drops/ml");
      display.println("");
      if (Case0Count == 2)
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
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setCursor(0, 0);
      display.println("Allowable Flow  rate Deviation  without activat-ing alarm       (+/- Percentage)");
      display.setTextColor(BLACK);
      display.print(" ");
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setTextSize(2);
      display.print(Case5Count);
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
      if (Case1Count == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else
        display.println("1) ug");
      display.println("");
      if (Case1Count == 1) {
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
      if (Case1Count == 0)
        display.println(Case2CountUg);
      else
        display.println(Case2CountMg);
      display.refresh();
      break;

    case drug_mass_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      display.println("Input Drug Mass Units of Measure");
      display.setTextColor(BLACK);
      if (Case1Count == 0) {
        display.setTextColor(WHITE, BLACK);
        display.println("1) ug");
        display.setTextColor(BLACK);
      }
      else
        display.println("1) ug");
      display.println("");
      if (Case1Count == 1) {
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
      if (Case1Count == 0)
      {
        display.println(Case2CountUg);
      }
      else
        display.println(Case2CountMg);
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
      display.println(Case3Count);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.println(Case4Count);
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
      display.println(Case3Count);
      display.setTextSize(1);
      display.println("");
      display.setTextColor(WHITE, BLACK);
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK);
      display.println(Case4Count);
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
  delay(6);
}

