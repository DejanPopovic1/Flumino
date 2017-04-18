#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <SPI.h>
#define SCK                      5
#define MOSI                     6
#define SS                       7
#define BLACK                    0
#define WHITE                    1

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
double DisplayedFlowRate;
double DrugFlowRate              = 0;
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
int dropsPerMillilitreSelector  = 1;
int inputDrugMassUOMSelector    = 0;
int drugMassUgSelector          = 100;
int drugMassMgSelector          = 1;
int patientMassSelector         = 65;
int volumeDilutantSelector      = 1000;
int allowableFlowRateSelector   = 12;

bool BuzzerState = false;
double newFlowRate = 180;
unsigned long previousTime = 0;
bool isDropPassing = false;

unsigned long button2PressedTime = 0;

void readPins(int *const, int *const, int *const, int *const, int *const, const int, const int, const int, const int, const int);

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
  readPins(&sensorReading, &button1State, &button2State, &button3State, &button4State, analogInPin, Button1InPin, Button2InPin, Button3InPin, Button4InPin);
  isDropPassing = evaluateSensor(previoiusSensorReading, sensorReading);
  evaluateFlowRate(isDropPassing, &DisplayedFlowRate, &previousTime, currentTime, dropsPerMillilitre, &newFlowRate, &period);
  DrugFlowRate = drugFlowRate(inputDrugMassUOMSelector, dose_shown, drugMassUgSelector, DisplayedFlowRate, volumeDilutantSelector, patientMassSelector, drugMassMgSelector);
  evaluateButton1(button1State, &lastButton1State, &button1PushCounter, show_dose);
  evaluateButton2(button2State, &lastButton2State, Menu, &dropsPerMillilitreSelector, &inputDrugMassUOMSelector, &drugMassMgSelector, &drugMassUgSelector, &patientMassSelector, &volumeDilutantSelector, &allowableFlowRateSelector, &show_dose, &dose_shown, &dropsPerMillilitre, &button2PressedTime);
  evaluateButton3(button3State, &lastButton3State, Menu, &dropsPerMillilitreSelector, &inputDrugMassUOMSelector, &drugMassMgSelector, &drugMassUgSelector, &patientMassSelector, &volumeDilutantSelector, &allowableFlowRateSelector, &show_dose, &dose_shown, &dropsPerMillilitre);
  evaluateButton4(Menu, button4State, &lastButton4State, &BuzzerState, &lower_sound_thresh, &upper_sound_thresh, &lower_drugsound_thresh, &upper_drugsound_thresh, newFlowRate, DrugFlowRate, allowableFlowRateSelector);
  evaluateBuzzer(BuzzerState, DisplayedFlowRate, lower_sound_thresh, upper_sound_thresh, BuzzerPin);
  previoiusSensorReading = sensorReading;
  display.clearDisplay();
  Menu = button1PushCounter;
  printToScreen(Menu, dropsPerMillilitreSelector, allowableFlowRateSelector, show_dose, dose_shown, inputDrugMassUOMSelector, drugMassUgSelector, drugMassMgSelector, patientMassSelector, volumeDilutantSelector,
                DisplayedFlowRate, BuzzerState, lower_sound_thresh, upper_sound_thresh, lower_drugsound_thresh, upper_drugsound_thresh);
  delay(6);
}

void readPins(int *const sensorReading, int *const button1State, int *const button2State, int *const button3State, int *const button4State, const int analogInPin, const int Button1InPin, const int Button2InPin,
              const int Button3InPin, const int Button4InPin) {
  if (analogRead(analogInPin) >= SENSOR_THRESHOLD){
    *sensorReading = HIGH;
  }
  else{
    *sensorReading = LOW;
  }
  *button1State = analogRead(Button1InPin);
  *button2State = analogRead(Button2InPin);
  *button3State = analogRead(Button3InPin);
  *button4State = digitalRead(Button4InPin);
}

