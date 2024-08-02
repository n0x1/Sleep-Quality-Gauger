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
  lcd.clear();  
  lcd.setCursor(0, 0); 
  lcd.print("On");

   dht.begin(); 

  // Serial1.begin(11520);  // Start serial communication bugging lcd maybe
}

void loop() { // enclose all in if so its only if switch is high

  int sliderValue = analogRead(SLIDER_PIN);

  motionState = digitalRead(MOTION_PIN);
   lightState = digitalRead(LDR_PIN);

  if (sliderValue > 700 && running == true) {

       lcd.clear(); 
        delay(1000);
    //motion
    if (motionWarning == false) {
      if (motionState == HIGH) {
        
        detectedMotion++;
        delay(3000); // high in case of extended toss and turn
      }
      if (detectedMotion >= motionCountTillWarning) {
        motionWarning = true;
        lcd.print("OVER MOTION LIMIT");
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
        lcd.print("BRIGHT");
      }
    }

  //temp
    if (tempHighWarning == false) {
      float celsius = dht.readTemperature();

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
      float celsius = dht.readTemperature();

      if (celsius < TEMP_THRESHOLD_LOW) {
          tempTooLowCount++;
          lcd.print("LOW TEMP WARNING");
          delay(1000);
      }
      if (tempTooLowCount >= tempLowCountTilWarning) {
          tempLowWarning = true;
  
      }

    }
    
    //humidity
        if (humidHighWarning == false) {
      float humidity = dht.readHumidity();

      if (humidity > HUMID_THRESHOLD_HIGH) {
          humidTooHighCount++;
           lcd.print("HIGH HUMID WARNING");
          delay(1000);
      }
      if (humidTooHighCount >= humidHighCountTilWarning) {
          humidHighWarning = true;
      }
    }
        if (humidLowWarning == false) {
      float humidity = dht.readHumidity();

      if (humidity < HUMID_THRESHOLD_LOW) {
          humidTooLowCount++;
          lcd.print("LOW HUMID WARNING");
          delay(1000);
      }
      if (humidTooLowCount >= humidLowCountTilWarning) {
          humidLowWarning = true;
  
      }

    }


  }

  if (!lightWarning && !motionWarning && !humidLowWarning && !humidHighWarning && !tempLowWarning && !tempHighWarning) {
    lcd.print("No issues"); // heart
    lcd.write(byte(0)); // heart
    lcd.write(byte(1)); // :)
    delay(1000);
  }

  delay(10);
}


// after 8 hours, alarm?