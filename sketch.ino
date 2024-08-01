#include <LiquidCrystal.h>
#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A'},
  { '4', '5', '6', 'B'},
  { '7', '8', '9', 'C'},
  { '*', '0', '#', 'D'}
};

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

uint8_t colPins[COLS] = { 21, 20, 19, 17 };
uint8_t rowPins[ROWS] = { 28, 27, 26, 22 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);  // Start serial communication
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  
  lcd.createChar(0, heart);
  

  lcd.print("On");
  Serial.println("Setup complete");
}

void loop() {
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    Serial.print("Key pressed: ");
    Serial.println(key);
    lcd.clear();
    lcd.print(key);
  }
  delay(10);

}