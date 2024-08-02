#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


#define LDR_PIN 2
#define MOTION_PIN 16
#define TEMP_PIN 26 // temperature 
#define SLIDER_PIN 27
#define DHT_PIN 4 // sda pin
#define BUZZER_PIN 17

DHT dht(DHT_PIN, DHT22);

const int ALARM_HOURS = 8; // IN HOURS, after going to sleep. 8 recomended
bool alarmNotStopped = true;
int cursorX = 0;

//motion
int detectedMotion = 0;
const int motionCountTillWarning = 5; // just in case user gets up before clock off , extra movements
bool motionWarning = false;
int motionState = LOW;

//light
int lightState = HIGH;
int isLit = 0;
bool lightWarning = false;
const int lightCountTilWarning = 10; // this may be caused by sunrise too ig, but you can calibrate on the sensor irl

//temp
bool tempHighWarning = false;
bool tempLowWarning = false;
int tempTooHighCount = 0;
int tempTooLowCount = 0;
const int tempHighCountTilWarning = 6;
const int tempLowCountTilWarning = 6;
const float TEMP_THRESHOLD_HIGH = 23.9; // (celsius) range can be changed but this is good for safety from research  
const float TEMP_THRESHOLD_LOW = 12.7;

//humidity
bool humidHighWarning = false;
bool humidLowWarning = false;
int humidTooHighCount = 0;
int humidTooLowCount = 0;
const int humidHighCountTilWarning = 6;
const int humidLowCountTilWarning = 6;
const int HUMID_THRESHOLD_HIGH = 50; // these ones are percents
const int HUMID_THRESHOLD_LOW = 30; // 30-50% is recommended 


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
uint8_t smiley[8] = {
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

void setup() {
  pinMode(MOTION_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(SLIDER_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

    lcd.begin(16, 2);
      lcd.createChar(0, heart);
      lcd.createChar(1, smiley);
    lcd.print("Slide UP to enable"); 
    lcd.write(byte(0)); // heart
    lcd.write(byte(1)); // :)
    delay(1000);
    lcd.clear();


   dht.begin(); 

  // setAlarm();
}

void loop() { // enclose all in if so its only if switch is high

  int sliderValue = analogRead(SLIDER_PIN);
  unsigned long time = millis();

  motionState = digitalRead(MOTION_PIN);
   lightState = digitalRead(LDR_PIN);

  if (sliderValue > 700 && running == true) {

    //motion
    if (motionWarning == false) {
      if (motionState == HIGH) {
        
        detectedMotion++;
        delay(3000); // high in case of extended toss and turn
      }
      if (detectedMotion >= motionCountTillWarning) {
        motionWarning = true;
        printWithFormatting("Motion");
      }
    }


    //light
    if (lightWarning == false) {
      if (lightState == LOW) {
        isLit++;
        delay(1000);
      }
      if (!lightWarning && isLit >= lightCountTilWarning) {
        lightWarning = true;

        lcd.print("Bright");
      }
    }

  //temp
    if (tempHighWarning == false) {
      float celsius = dht.readTemperature();

      if (celsius > TEMP_THRESHOLD_HIGH) {
          tempTooHighCount++;
          delay(1000);
      }
      if (!tempHighWarning && tempTooHighCount >= tempHighCountTilWarning) {
          tempHighWarning = true;
          lcd.print("Hot");
      }
    }
        if (tempLowWarning == false) {
      float celsius = dht.readTemperature();

      if (celsius < TEMP_THRESHOLD_LOW) {
          tempTooLowCount++;
          
          delay(1000);
      }
      if (!tempLowWarning && tempTooLowCount >= tempLowCountTilWarning) {
          tempLowWarning = true;
          lcd.print("Cold");
      }

    }
    
    //humidity
        if (humidHighWarning == false) {
      float humidity = dht.readHumidity();

      if (humidity > HUMID_THRESHOLD_HIGH) {
          humidTooHighCount++;
          delay(1000);
      }
      if (!humidHighWarning && humidTooHighCount >= humidHighCountTilWarning) {
          humidHighWarning = true;
          lcd.print("Humid");
      }
    }
        if (humidLowWarning == false) {
      float humidity = dht.readHumidity();

      if (humidity < HUMID_THRESHOLD_LOW) {
          humidTooLowCount++;

      }
      if (humidLowWarning == false && humidTooLowCount >= humidLowCountTilWarning) {
          humidLowWarning = true;
          lcd.print("Dry");
  
      }

    }


  }

 
 int frequencies[] = { 523, 587, 659, 698, 784, 880, 987, 1046 };

  // Play each note in the tune
if (!running && sliderValue > 700) {
  for (int i = 0; i < 8; i++) {
    tone(BUZZER_PIN, frequencies[i], 15);
    delay(100); 
  } // alarm
  delay(1000);
}


  delay(10);
}


void setAlarm() {
  
  delay(ALARM_HOURS * 60 * 60 * 1000); // to ms, 8 hours rec 

    running = false; // this triggers alarm in loop()


}


void printWithFormatting(const char* text) {
    int length = 0;

  if (cursorX == 0) {
    lcd.print("Warnings: ");
    lcd.setCursor(0,1);
    lcd.print(text);
    while (text[length] != '\0') {
        length++;
    }
  } else {
    lcd.print(", ");
    lcd.print(text);
    while (text[length] != '\0') {
        length++;
    }
  } 


  cursorX = length;
}