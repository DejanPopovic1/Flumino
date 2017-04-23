#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <SPI.h>
#define SCK                      5
#define MOSI                     6
#define SS                       7
#define BLACK                    0
#define WHITE                    1

#define SENSOR_THRESHOLD 600

#define BUTTON_HELD_TRIGGER_INCREMENT_TIME 500000
#define BUTTON_HELD_INCREMENT_TIME 500000

const int analogInPin           = A1;
const int BuzzerPin             = 3;
const int Button1InPin          = A0;
const int Button2InPin          = A2;
const int Button3InPin          = A3;
const int Button4InPin          = 2;
const int IROutPin              = 12;

Adafruit_SharpMem display(SCK, MOSI, SS);

enum button_state {
  BUTTON_PRESSED,
  BUTTON_RELEASED,
  BUTTON_HELD,
  BUTTON_UNTOUCHED,
  BUTTON_INCREMENTED
};

enum sensor_state {
  DROP_PASSING,
  DROP_NOT_PASSING
};

struct MachineState {
  int Menu                        = 0;
  double DisplayedFlowRate        = 180;
  double DrugFlowRate             = 0;
  unsigned long period            = 0;
  int dropsPerMillilitre          = 20;
  int show_dose                   = false;
  int dose_shown                  = 1;
  float lower_sound_thresh        = 0;
  float upper_sound_thresh        = 0;
  float lower_drugsound_thresh    = 0;
  float upper_drugsound_thresh    = 0;
  int sensorStatus                = 0;
  int lastSensorState             = 0;
  int sensorState                 = 0;
  int button1PushCounter          = 8;
  int button1State                = 0;
  int lastButton1State            = 0;
  int button1Status               = BUTTON_UNTOUCHED;
  int button2State                = 0;
  int lastButton2State            = 0;
  int button2Status               = BUTTON_UNTOUCHED;
  unsigned long button2HeldTime   = 0;
  unsigned long button2PressedTime= 0;
  int button3State                = 0;
  int lastButton3State            = 0;
  int button3Status               = BUTTON_UNTOUCHED;
  unsigned long button3HeldTime   = 0;
  unsigned long button3PressedTime= 0;
  int button4State                = 0;
  int lastButton4State            = 1;
  int button4Status               = BUTTON_UNTOUCHED;
  int dropsPerMillilitreSelector  = 1;
  int inputDrugMassUOMSelector    = 0;
  int drugMassUgSelector          = 100;
  int drugMassMgSelector          = 1;
  int patientMassSelector         = 65;
  int volumeDilutantSelector      = 1000;
  int allowableFlowRateSelector   = 12;
  bool BuzzerState                = false;
  double flowRate                 = 180;
  unsigned long previousTime      = 0;
  unsigned long currentTime       = 0;
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

struct MachineState currentMachineState;
int (*incrementFunctionPointer)(int, int) = &smallIncrement;
int (*decrementFunctionPointer)(int, int) = &smallDecrement;

void setup()
{
  display.begin();
  Serial.begin(9600);
  digitalWrite(12, HIGH);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(IROutPin, OUTPUT);
}

void loop()
{
  readState(&currentMachineState);
  evaluateFlowRate(&currentMachineState);
  drugFlowRate(&currentMachineState);
  evaluateButton1(&currentMachineState);
  evaluateButton2(&currentMachineState, incrementFunctionPointer);
  evaluateButton3(&currentMachineState, decrementFunctionPointer);
  evaluateButton4(&currentMachineState);
  evaluateBuzzer(&currentMachineState);
  printToScreen(&currentMachineState);
  saveState(&currentMachineState);
}

void readState(struct MachineState *s) {
  s->currentTime = micros();
  readPins(s);
  buttonStatus(&s->lastButton2State, &s->button2State, &s->button2Status, &incrementFunctionPointer, &s->button2HeldTime, &s->currentTime, &s->button2PressedTime, 2);
  buttonStatus(&s->lastButton3State, &s->button3State, &s->button3Status, &decrementFunctionPointer, &s->button3HeldTime, &s->currentTime, &s->button3PressedTime, 3);
//  buttonStatus(/*button3*/);
//  buttonStatus(/*button4*/);
  sensorStatus(s);
}

void buttonStatus(int *lastButtonState, int *buttonState, int *buttonStatus, int (**functionPointer)(int, int), unsigned long *buttonHeldTime, unsigned long *currentTime, unsigned long *buttonPressedTime, int button) {
  if (*lastButtonState == LOW && *buttonState != LOW) {
    *buttonStatus = BUTTON_RELEASED;
    *functionPointer = NULL;
  }
  else if (*lastButtonState == LOW && *buttonState == LOW) {
    *buttonHeldTime = *currentTime;
    *buttonStatus = BUTTON_HELD;
    *functionPointer = NULL;
    if (*buttonHeldTime - *buttonPressedTime >= BUTTON_HELD_TRIGGER_INCREMENT_TIME) {
      *buttonStatus = BUTTON_INCREMENTED;
      *buttonPressedTime = *currentTime;
      (button == 2) ? (*functionPointer = &largeIncrement) : (*functionPointer = &largeDecrement);
    }
  }
  else if (*lastButtonState != LOW && *buttonState == LOW) {
    *buttonStatus = BUTTON_PRESSED;
    *buttonPressedTime = *currentTime;
    (button == 2) ? (*functionPointer = &smallIncrement) : (*functionPointer = &smallDecrement);
  }
  else if (*lastButtonState != LOW && *buttonState != LOW) {
    *buttonStatus = BUTTON_UNTOUCHED;
  }
}

void sensorStatus(struct MachineState *s) {
  static unsigned long i = 0;
  if (s -> lastSensorState >= SENSOR_THRESHOLD && s -> sensorState < SENSOR_THRESHOLD) {
    s -> sensorStatus = DROP_PASSING;
    Serial.println(++i);
  }
  else {
    s -> sensorStatus = DROP_NOT_PASSING;
  }
}

void saveState(struct MachineState *currentMachineState) {
  currentMachineState -> lastSensorState  = currentMachineState -> sensorState;
  currentMachineState -> lastButton1State = currentMachineState -> button1State;
  currentMachineState -> lastButton2State = currentMachineState -> button2State;
  currentMachineState -> lastButton3State = currentMachineState -> button3State;
  currentMachineState -> lastButton4State = currentMachineState -> button4State;
}

void readPins(struct MachineState *currentMachineState) {
  currentMachineState -> sensorState = analogRead(analogInPin);
  currentMachineState -> button1State = analogRead(Button1InPin);
  currentMachineState -> button2State = analogRead(Button2InPin);
  currentMachineState -> button3State = analogRead(Button3InPin);
  currentMachineState -> button4State = digitalRead(Button4InPin);
}
