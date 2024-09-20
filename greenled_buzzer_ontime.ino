#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

struct Item {
  char RFID[13];
  double price;
  int quantity;
};

Item items[] = {
  {"090008D09948", 10.00, 0}, // Butter
  {"400035606174", 20.00, 0} // Milk
  // Add more items here as needed
};

const int numItems = sizeof(items) / sizeof(items[0]);

const int redLedPin = 7;
const int greenLedPin = 8;
const int buzzerPin = 9;

double total = 0;
int count_prod = 0;

unsigned long lastCardReadTime = 0;
const unsigned long cardReadInterval = 10000; // 10 seconds

void setup() {
  pinMode(A4, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.clear();
  Wire.begin();
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print(" AUTOMATIC BILL");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print(" SHOPPING CART ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("SUPER MARKET");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please add item");
}

void loop() {
  char input[13];
  int count = 0;

  while (Serial.available() && count < 12) {
    input[count] = Serial.read();
    count++;
    delay(5);
  }
  input[count] = '\0'; // Null-terminate the input

  int a = digitalRead(A4);
  bool itemFound = false;

  for (int i = 0; i < numItems; i++) {
    if ((strncmp(input, items[i].RFID, 12) == 0) && (a == 1)) {
      lcd.setCursor(0, 0);
      lcd.print("Item Added         ");
      lcd.setCursor(0, 1);
      lcd.print("Price: ");
      lcd.print(items[i].price);
      items[i].quantity++;
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(2000);
      total += items[i].price;
      count_prod++;
      digitalWrite(greenLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      itemFound = true;
      break; // Exit the loop once the item is found
    }
  }

  if (!itemFound && (a == 0)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Item Not in Cart! ");
    digitalWrite(redLedPin, HIGH);
    delay(2000);
    digitalWrite(redLedPin, LOW);
    lcd.clear();
  }

  // Check if there hasn't been a card read for 10 seconds
  unsigned long currentTime = millis();
  if ((currentTime - lastCardReadTime) >= cardReadInterval) {
    // Display total price on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Total Price: ");
    lcd.print(total);
    total = 0;
    count_prod = 0;
  }

  // Store the last card read time
  lastCardReadTime = currentTime;
}