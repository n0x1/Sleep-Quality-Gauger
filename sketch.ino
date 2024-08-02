#include <LiquidCrystal.h>
#define LDR_PIN 2
#define MOTION_PIN 16
#define TEMP_PIN 26 // temperature 

int motionState = LOW;            
int detectedMotion = 0;     
const int motionCountTillWarning = 3;
bool motionWarning = false;


int lightState = HIGH;
int isLit = 0;
bool lightWarning = false;
const int lightCountTilWarning = 5;


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
  Serial1.begin(11520);  // Start serial communication
  lcd.begin(16, 2);
  
  lcd.createChar(0, heart);
  

  lcd.print("On");


  Serial1.println("Setup complete");

if (isLit != 0) {
  isLit = 0;
}
if (detectedMotion != 0) {
  detectedMotion = 0;
}
  
}

void loop() { // enclose all in if so its only if button is pressed
  // motion
motionState = digitalRead(MOTION_PIN);
  lcd.setCursor(2, 0);
if (motionWarning == false) {
  if (motionState == HIGH) {
    lcd.print("MOTION DETECTED");
    detectedMotion++;
  }
  if (detectedMotion >= motionCountTillWarning) {
    motionWarning = true;
    lcd.print("OVER MOTION LIMIT");
  }
}


  //light
lightState = digitalRead(LDR_PIN);
if (lightWarning == false) {
  if (lightState == LOW) {
    isLit++;
    lcd.print(isLit);
  }
  if (isLit >= lightCountTilWarning) {
      lightWarning = true;
      
  }
}

  


  delay(200);

}