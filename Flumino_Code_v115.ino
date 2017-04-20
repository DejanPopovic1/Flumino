#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <SPI.h>
#define SCK                      5
#define MOSI                     6
#define SS                       7
#define BLACK                    0
#define WHITE                    1

#define SENSOR_THRESHOLD 600

const int analogInPin           = A1;  // Analog input pin that the potentiometer is attached to
const int BuzzerPin             = 3;

Adafruit_SharpMem display(SCK, MOSI, SS);  //For the SPI BUS

struct MachineState {
  int Menu;
  int previoiusSensorReading      = 0;     // previous state of the button
  double DisplayedFlowRate;
  double DrugFlowRate             = 0;
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

  
  unsigned long button2PressedTime = 0;
  unsigned long currentTime;
};

enum menu_page {
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

void readState(int *const, int *const, int *const, int *const, int *const, const int, const int, const int, const int, const int);

bool isDropPassing = false;

struct MachineState currentMachineState;

void setup()
{
  display.begin();
  Serial.begin(9600);
  digitalWrite(12, HIGH);
  pinMode(13, OUTPUT); //LED
  pinMode(3, OUTPUT);   //Buzzer
  pinMode(2, INPUT);   //Button 4
  pinMode(currentMachineState.IROutPin, OUTPUT); //Infrared LED
}


void loop()
{
  currentMachineState.currentTime = micros();
  readState(&currentMachineState);
  delayMicroseconds(1);
  isDropPassing = evaluateSensor(&currentMachineState);
  evaluateFlowRate(isDropPassing, &currentMachineState);
  drugFlowRate(&currentMachineState);
  evaluateButton1(&currentMachineState);
  evaluateButton2(&currentMachineState);
  evaluateButton3(&currentMachineState);
  evaluateButton4(&currentMachineState);
  evaluateBuzzer(&currentMachineState);
  display.clearDisplay();
  currentMachineState.Menu = currentMachineState.button1PushCounter;
  printToScreen(&currentMachineState);
  delay(6);
  currentMachineState.previoiusSensorReading = currentMachineState.sensorReading;
  saveState(&currentMachineState);
}


void readState(struct MachineState *currentMachineState) {
  readPins(currentMachineState);
}

void saveState(struct MachineState *currentMachineState) {
  currentMachineState -> lastButton1State = currentMachineState -> button1State;
  currentMachineState -> lastButton2State = currentMachineState -> button2State;
  currentMachineState -> lastButton3State = currentMachineState -> button3State;
  currentMachineState -> lastButton4State = currentMachineState -> button4State;
}

void readPins(struct MachineState *currentMachineState) {
  if (analogRead(analogInPin) >= SENSOR_THRESHOLD) {
    currentMachineState -> sensorReading = HIGH;
  }
  else {
    currentMachineState -> sensorReading = LOW;
  }
  currentMachineState -> button1State = analogRead(currentMachineState -> Button1InPin);
  currentMachineState -> button2State = analogRead(currentMachineState -> Button2InPin);
  currentMachineState -> button3State = analogRead(currentMachineState -> Button3InPin);
  currentMachineState -> button4State = digitalRead(currentMachineState -> Button4InPin);
}
