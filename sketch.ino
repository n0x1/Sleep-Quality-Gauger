#include <LiquidCrystal.h>
#define LDR_PIN 2
#define MOTION_PIN 16
#define TEMP_PIN 26 // temperature 
#define SLIDER_PIN 27

//motion
int detectedMotion = 0;
const int motionCountTillWarning = 5; // just in case user gets up before clock off , extra movements
bool motionWarning = false;
int motionState = LOW;

//light
int lightState = HIGH;
int isLit = 0;
bool lightWarning = false;
const int lightCountTilWarning = 7;

bool tempHighWarning = false;
bool tempLowWarning = false;
int tempTooHighCount = 0;
int tempTooLowCount = 0;
const int tempHighCountTilWarning = 6;
const int tempLowCountTilWarning = 6;
const float TEMP_THRESHOLD_HIGH = 23.9; // (celsius) range can be changed but this is good for safety from research  
const float TEMP_THRESHOLD_LOW = 12.7;
const float BETA = 3950; // for this specific thermistor



bool running = true; // disable to stop the loop



LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

uint8_t heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
};

void setup() {
  pinMode(MOTION_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(SLIDER_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
    lcd.begin(16, 2);
      lcd.createChar(0, heart);
  lcd.clear();  
  lcd.setCursor(0, 0); 
  lcd.print("On");

  // Serial1.begin(11520);  // Start serial communication bugging lcd maybe



  isLit = 0;
  detectedMotion = 0;
  running = true;
}

void loop() { // enclose all in if so its only if switch is high

  int sliderValue = analogRead(SLIDER_PIN);

  motionState = digitalRead(MOTION_PIN);
   lightState = digitalRead(LDR_PIN);

  if (sliderValue > 700 && running = true;) {
       lcd.clear();  
    //motion
    if (motionWarning == false) {
      if (motionState == HIGH) {
        
        detectedMotion++;
        delay(3000); // in case of extended toss and turn
      }
      if (detectedMotion >= motionCountTillWarning) {
        motionWarning = true;
        lcd.println("OVER MOTION LIMIT");
      }
    }


    //light
    if (lightWarning == false) {
      if (lightState == LOW) {
        isLit++;
        delay(1000);
      }
      if (isLit >= lightCountTilWarning) {
        lightWarning = true;

      }
    }

    if (tempHighWarning == false) {
      int thermistorReading = analogRead(TEMP_PIN);
      float celsius = 1 / (log(1 / (1023. / thermistorReading - 1)) / BETA + 1.0 / 298.15) - 273.15;

      if (celsius > TEMP_THRESHOLD_HIGH) {
          tempTooHighCount++;
           lcd.print("HIGH TEMP WARNING");
          delay(1000);
      }
      if (tempTooHighCount >= tempHighCountTilWarning) {
          tempHighWarning = true;
      }
    }
        if (tempLowWarning == false) {
      int thermistorReading = analogRead(TEMP_PIN);
      float celsius = 1 / (log(1 / (1023. / thermistorReading - 1)) / BETA + 1.0 / 298.15) - 273.15;

      if (celsius < TEMP_THRESHOLD_LOW) {
          tempTooLowCount++;
          lcd.print("LOW TEMP WARNING");
          delay(1000);
      }
      if (tempTooLowCount >= tempLowCountTilWarning) {
          tempLowWarning = true;
  
      }

    }


  }

  delay(10);
}


// after 8 hours, disabale