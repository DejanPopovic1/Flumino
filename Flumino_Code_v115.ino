//------Library------------------------------
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <SPI.h>
//-------Define-----------------------------
#define SCK                      5
#define MOSI                     6
#define SS                       7
#define BLACK                    0
#define WHITE                    1
#define CONTACT_THRESHOLD        900
#define TOUCH_THRESHOLD          999
#define ON                       1
#define OFF                      0
#define DISPLAY_PERIOD           2000000
//#define MILLILITRES_PER_DROP 20
Adafruit_SharpMem display(SCK, MOSI, SS);  //For the SPI BUS
//------------Variables-----------------------//

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
//-------------States-//
int buttonPushCounter          = 0;   // counter for the number of button presses
int buttonState                = 0;         // current state of the button
int lastButtonState            = 0;     // previous state of the button
int LastbuttonPushCounter      = 0;
//---------------FLOW-Variables----------------
float SetFlowRate              = 0;
int state                      = 1013;
int lastState                  = 1013;
float timelatest       = 0;
float timelatest2 = 0;

float FlowRate                 = 180;
float DisplayedFlowRate;
float DrugFlowRate             = 0;
float timeoldest       = 0;
float period           = 0;
int i;
int touch_state                = 0;
int touch_lastState            = 0;
unsigned long touch_timelatest = 0;
unsigned long touch_timeoldest = 0;
unsigned long buzzer_time      = 0;
int millilitres_per_drop       = 20;
int show_dose                  = false;
int dose_shown                 = 1;
int set_alarm_flow = 0;
float lower_sound_thresh = 0;
float upper_sound_thresh = 0;
float lower_drugsound_thresh = 0;
float upper_drugsound_thresh = 0;
int FlowField = 0;
//------------------------------------------

float  period2 = 1;
float      dph = 1;
float      mpd = 1 ;
//------------------------------------------
// Variables will change:
int DigitalReading             = 0;
int inPin                      = 7;         // the number of the input pin
int IROutPin                   = 12;         // the number of the input pin
int outPin                     = 13;       // the number of the output pin
int counter                    = 0;       // how many times we have seen new value
int reading;           // the current value read from the input pin
int current_state              = LOW;    // the debounced input value// the following variable is a long because the time, measured in milliseconds,// will quickly become a bigger number than can be stored in an int.
int times                      = 0;         // the last time the output pin was sampled
int debounce_count             = 3; // number of millis/samples to consider before declaring a debounced input
//----------Variables-Declarelations-----
int a                          = 2;   //declaring integer
const int analogInPin          = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin         = 9; // Analog output pin that the LED is attached to

float sensorValue              = 0;        // value read from the pot
int outputValue                = 0;        // value output to the PWM (analog out)
int BuzzerPin                  = 3;
//-------Signal-Testing-----//
int y, countz, newcountz;
int lastcountz                 = 0;
double stddev1;
double AdThres;
double Average;
int peakValue                  = 0;
float filterVal;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float smoothedVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing
float smoothedVal2;   // this would be the buffer value for another sensor if you needed to smooth two different sensors - not used in this sketch
float AmpedSmoothedVal;
int mainPeak;
//------Button-1-Code-----------------
const int Button1InPin      = A0;  // Analog input pin that the potentiometer is attached to
int Button1                 = 0;        // value read from the button
int button1PushCounter      = 8;   // counter for the number of button presses
int button1State            = 0;         // current state of the button
int lastButton1State        = 0;     // previous state of the button
//------Button-2-Code-----------------
const int Button2InPin      = A2;  // Analog input pin that the potentiometer is attached to
int Button2                 = 0;        // value read from the button
int button2PushCounter      = 1;   // counter for the number of button presses
int button2State            = 0;         // current state of the button
int lastButton2State        = 0;     // previous state of the button
//------Button-3-Code-----------------
const int Button3InPin      = A3;  // Analog input pin that the potentiometer is attached to
int Button3                 = 0;        // value read from the button
int button3PushCounter      = 1;   // counter for the number of button presses
int button3State            = 0;         // current state of the button
int lastButton3State        = 0;     // previous state of the button
//------Button-4-Code-----------------
const int Button4InPin      = 2;  // Analog input pin that the potentiometer is attached to
int Button4                 = 0;        // value read from the button
int button4PushCounter      = false;   // counter for the number of button presses
int button4State            = 0;         // current state of the button
int lastButton4State        = 1;     // previous state of the button - Digital Pin

//--------Menu-Variable-Counters-------//
int Case0Count       = 1;
int Case1Count       = 0;
int Case2CountUg     = 100;
int Case2CountMg     = 1;
int Case3Count       = 65;
int Case4Count       = 1000;
int Case5Count       = 12;

int Case2editCount   = 1;
int Case3editCount   = 1;
//----------------Exponential-Smoothing-----//
float alpha = 0.25;
float AverageFlowRate = 180;
int BuzzerState = 0;



//==========================================================//
//=================Setup=CODE===============================//
//==========================================================//

void setup()
{
  //------------Initialize Display------------//
  display.begin();
  //--------------------------------------
  //countz = 0;

  char b[2];   //declaring character array
  String str;  //declaring string
  Serial.begin(9600);
  //-------------------------------------
  pinMode(13, OUTPUT); //LED
  pinMode(3, OUTPUT);   //Buzzer
  pinMode(2, INPUT);   //Button 4
  pinMode(IROutPin, OUTPUT); //Infrared LED
  //---------------------------------------
}

unsigned long current_time_beginning_loop;
unsigned long current_time_ending_loop;

//==========================================================//
//=================LOOP=CODE===============================//
//==========================================================//

void loop()
{
  
current_time_beginning_loop = micros();
  
//Serial.println(micros());

  //-S-1.0------------Analog Read-------------------//
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)

  sensorValue = analogRead(analogInPin);
  delayMicroseconds(1);
  //digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
  //---------------Filters-----------------------//
  smoothedVal =  sensorValue;
  AmpedSmoothedVal = smoothedVal;
  AdThres = 600;  // Main Parameter- Set threshold Value;
  //-------Logical-Sensing-----------------------//

  if (sensorValue >= AdThres) //Threshold
  {
    DigitalReading = HIGH;
  }
  else
  {
    DigitalReading = LOW;
  }
  digitalWrite(13, DigitalReading );   // turn the LED on (HIGH is the voltage level)
  //----------State-Machine---------------------

  buttonState = DigitalReading;
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
    }
    else {
    }
  }
  lastButtonState = buttonState;

  //===============Button1=Code=========//
  Button1 = analogRead(Button1InPin);
  button1State = Button1;
  if (button1State != lastButton1State && button1State == 0 && show_dose == true) {
    if (button1PushCounter == 8)
      button1PushCounter = 0;
    else
      button1PushCounter++;
  }

  else if (button1State != lastButton1State && button1State == 0 && show_dose == false) {
    if (button1PushCounter == 8)
      button1PushCounter = 0;
    else if (button1PushCounter == 2)
      button1PushCounter = 8;
    else
      button1PushCounter++;
  }
  lastButton1State = button1State;

  //===============Button2=Code=========// Select Down
  Button2 = analogRead(Button2InPin);
  button2State = Button2;
  int button2initialtime = micros();
  if (button2State != lastButton2State && button2State == 0) {
    if (Menu == 0)
      if (Case0Count >= 2)
        Case0Count = 0;
      else
        Case0Count++;

    else if (Menu == 4)
      if (Case1Count >= 1)
        Case1Count = 0;
      else
        Case1Count++;

    else if (Menu == drug_mass_page && Case1Count == 0)
      if (Case2CountUg >= 100)
        Case2CountUg = 0;
        
        else if (micros() - button2initialtime >= 1000000){
          Serial.println("Beep");
          //Case2CountUg = Case2CountUg + 10;
          //button2initialtime = micros();
        }
        
      else
        Case2CountUg++;

    else if (Menu == 5 && Case1Count == 1)
      if (Case2CountMg >= 100)
        Case2CountMg = 0;
      else
        Case2CountMg++;

    else if (Menu == 6)
      if (Case3Count >= 500)
        Case3Count = 0;
      else
        Case3Count++;

    else if (Menu == 7)
      if (Case4Count >= 1000)
        Case4Count = 0;
      else
        Case4Count++;

    else if (Menu == 1)
      if (Case5Count >= 100)
        Case5Count = 0;
      else
        Case5Count++;

    else if (Menu == 2)
      if (show_dose == true)
        show_dose = false;
      else
        show_dose = true;

    else if (Menu == 3)
      if (dose_shown >= 2)
        dose_shown = 0;
      else
        dose_shown ++;

    delay(1);
  }

  lastButton2State = button2State;

  //===============Button3=Code=========// Select Up
  Button3 = analogRead(Button3InPin);
  button3State = Button3;
  if (button3State != lastButton3State && button3State == 0) {
    if (Menu == 0)
      if (Case0Count == 0)
        Case0Count = 2;
      else
        Case0Count--;

    else if (Menu == 4)
      if (Case1Count == 0)
        Case1Count = 1;
      else
        Case1Count--;




    else if (Menu == 5 && Case1Count == 0)
      if (Case2CountUg == 0)
        Case2CountUg = 100;
      else
        Case2CountUg--;
    else if (Menu == 5 && Case1Count == 1)
      if (Case2CountMg == 0)
        Case2CountMg = 100;
      else
        Case2CountMg--;


    else if (Menu == 6)
      if (Case3Count == 0)
        Case3Count = 500;
      else
        Case3Count--;

    else if (Menu == 7)
      if (Case4Count == 0)
        Case4Count = 1000;
      else
        Case4Count--;

    else if (Menu == 1)
      if (Case5Count == 0)
        Case5Count = 100;
      else
        Case5Count--;

    else if (Menu == 2)
      if (show_dose == true)
        show_dose = false;
      else
        show_dose = true;

    else if (Menu == 3)
      if (dose_shown == 0)
        dose_shown = 2;
      else
        dose_shown --;

    delay(1);
  }
  lastButton3State = button3State;




  //===============Button4=Code=========//
  Button4 = digitalRead(Button4InPin);
  button4State = Button4;
  if (button4State != lastButton4State && button4State == LOW) {
    
    if (BuzzerState == 0)
        BuzzerState = 1 ;
    else
        BuzzerState = 0 ;  

      lower_sound_thresh = AverageFlowRate * (1 - (float)Case5Count / 100);
      upper_sound_thresh = AverageFlowRate * (1 + (float)Case5Count / 100);
      lower_drugsound_thresh = DrugFlowRate * (1 - (float)Case5Count / 100);
      upper_drugsound_thresh = DrugFlowRate * (1 + (float)Case5Count / 100);
    }
    lastButton4State = button4State;

 
  //------------------------------------------------//
  if (BuzzerState == 1 )
  {
    if ((AverageFlowRate < lower_sound_thresh  || AverageFlowRate > upper_sound_thresh) ) {
      //Serial.println("Beep");
      digitalWrite(BuzzerPin, HIGH );

    }
    else {
      digitalWrite(BuzzerPin, LOW );
    }
  }
  else
  {
    digitalWrite(BuzzerPin, LOW );
  }






  //-----------------------------------------------------Internal Variables------------------//
  //-----------------------------------------------------Internal Variables------------------//
  //-----------------------------------------------------Internal Variables------------------//
  switch (Case0Count) {
    case 0:
      millilitres_per_drop = 10;
      break;
    case 1:
      millilitres_per_drop = 20;
      break;
    case 2:
      millilitres_per_drop = 60;
      break;
  }







  //-----------------------------------------------------Internal Variables------------------//
  //-----------------------------------------------------Internal Variables------------------//
  //-----------------------------------------------------Internal Variables------------------//


  //-S1.1------Drip Logic----------

  
  if (buttonPushCounter != LastbuttonPushCounter)
  {
    CalculateFlowRate();
    timelatest2 = timelatest;
  }
      LastbuttonPushCounter = buttonPushCounter;
  
  if(current_time_beginning_loop >= timelatest2 + period){
    timelatest2 = current_time_beginning_loop;
    CalculateDecayedFlowRate();
    }

  if (current_time_beginning_loop % DISPLAY_PERIOD <= DISPLAY_PERIOD/100){
  DisplayedFlowRate = AverageFlowRate;
    }







  
  //FlowRate = 180;
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


  //------------DEBUG--------------//

//  Serial.print(sensorValue);
//  Serial.print(">");
//  Serial.print(AverageFlowRate);
//  Serial.print("<");
//  Serial.print(DigitalReading);
//  Serial.print("%");
//  Serial.print(FlowRate);
//  Serial.print(",");
//  Serial.println();
//  Serial.println(micros());
//  
//  
//  Serial.print(micros());
//  Serial.print("    ");
//  Serial.print(timeoldest);
//  Serial.print("    ");
//  Serial.print(timelatest);
//  Serial.println("");  
  Serial.println(sensorValue);   
  Serial.println(micros());  
  Serial.println("");
  //------------------------------//


  //-S-2.0------------Convert-to-String-------------//

  //-S-3.0------------LCD-Display-------------------//
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
      //display.println("");
      display.print(" ");
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setTextSize(2);
      display.print(Case5Count);
//      display.println("");
//      display.println("");
//      display.setTextSize(1);
//      display.println("Alarm State: ");
//      if (BuzzerState == 1) {
//        display.setTextColor(BLACK);
//        display.print("ON");
//        display.print("  ");
//        display.setTextColor(WHITE);
//        display.println("OFF");
//        
//        display.println(" ");
//      }
//      else {
//        display.setTextColor(WHITE);
//        display.print("ON");
//        display.print("  ");
//        display.setTextColor(BLACK);
//        display.println(" OFF ");
//        display.setTextColor(BLACK);
//        display.println(" ");
//      }

      display.refresh();

      break;


    case input_dosage_y_n_page:

      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 0);
      //   display.println("Input Drug Mass Units of Measure");
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
      //   display.println("Input Drug Mass Units of Measure");
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
      //display.println("");
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
      //display.println("");
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
      // Serial.println();
      display.refresh();


      break;


    case patient_mass_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setCursor(0, 0);
      display.print("  Patient Mass        (kg)      ");
      display.setTextColor(BLACK);
      display.println(" ");
      //display.println("");
      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK);
      display.println(Case3Count);
      display.setTextSize(1);
      display.println("");
      // display.println(" <0.1-500kg>");
      // display.println("");



      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);

      display.setTextSize(2);
      display.print(" ");
      display.println(Case4Count);
      display.println("");

      display.refresh();


      //        setBitmap(mass); // Show Drop BMP to LCD

      break;

    case volume_dilutent_page:
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.setCursor(0, 0);
      display.print("  Patient Mass        (kg)      ");
      display.setTextColor(BLACK);
      display.println(" ");
      //display.println("");
      display.setTextSize(2);
      display.print(" ");
      //display.setTextColor(WHITE, BLACK);
      display.println(Case3Count);
      display.setTextSize(1);
      display.println("");
      // display.println(" <0.1-500kg>");
      // display.println("");



      display.setTextColor(WHITE, BLACK); // 'inverted' text
      display.println("Volume Dilutent       (ml)      ");
      display.setTextColor(BLACK);

      display.setTextSize(2);
      display.print(" ");
      display.setTextColor(WHITE, BLACK); // 'inverted' text
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
      //ADDED
      display.setTextSize(1);

      if (BuzzerState == 1){
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
        //ADDED
        display.setTextSize(1);
        //display.println("Sound off for: ");
        if (BuzzerState == 1){
        display.print(lower_drugsound_thresh);
        display.print("-");
        display.print(upper_drugsound_thresh);
        //ADDED
        }
      }

      else
        ;

      display.refresh();



      break;

    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }







  delay(1);

//Serial.println(micros());
//Serial.println("");
//Serial.println("");

  //----------END-------------------------------------//

current_time_ending_loop = micros();
}


//================Functions======================================

void CalculateDecayedFlowRate(void) {
      dph = (60 * 60 * 1000) / (current_time_beginning_loop-timelatest);
      mpd = (float)1 / millilitres_per_drop;
      FlowRate = dph * mpd * 1000;
      AverageFlowRate = alpha * FlowRate + (1 - alpha) * AverageFlowRate; 
}

void CalculateFlowRate(void) {

    timelatest = (float)current_time_beginning_loop;
    period = timelatest - timeoldest;
    if ((period) < 20)
      ;
    else {
      period2 = period;
      dph = (60 * 60 * 1000) / period2;
      mpd = (float)1 / millilitres_per_drop;
      FlowRate = dph * mpd * 1000;
      timeoldest = timelatest;
      AverageFlowRate = alpha * FlowRate + (1 - alpha) * AverageFlowRate;
    }
}


int smooth(int data, float filterVal, float smoothedVal) {


  if (filterVal > 1) {     // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0) {
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}


///----LCD FUNCTIONS-----------------------------------------//

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
    //display.println();
  }
  display.refresh();
}

void testdrawcircle(void) {
  for (uint8_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2 - 5, display.height() / 2 - 5, i, BLACK);
    display.refresh();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (uint8_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.refresh();
    color++;
  }
}

void testdrawtriangle(void) {
  for (uint16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, BLACK);
    display.refresh();
  }
}

void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.refresh();
  }
}

void testdrawroundrect(void) {
  for (uint8_t i = 0; i < display.height() / 4; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, BLACK);
    display.refresh();
  }
}

void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (uint8_t i = 0; i < display.height() / 4; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.refresh();
  }
}

void testdrawrect(void) {
  for (uint8_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, BLACK);
    display.refresh();
  }
}

void testdrawline() {
  for (uint8_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, BLACK);
    display.refresh();
  }
  for (uint8_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (uint8_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, BLACK);
    display.refresh();
  }
  for (int8_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (int8_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
    display.refresh();
  }
  for (int8_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (uint8_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, BLACK);
    display.refresh();
  }
  for (uint8_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK);
    display.refresh();
  }
  delay(250);
}
