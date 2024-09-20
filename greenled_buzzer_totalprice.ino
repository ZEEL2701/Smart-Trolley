#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

struct Item {
  char RFID[13];
  char name[20]; // Add the item name field
  double price;
  int quantity;
};

Item items[] = {
  {"090008D09948", "Butter", 10.00, 0},
  {"400035606174", "Milk", 20.00, 0}
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
  lcd.begin(16, 2);
  lcd.print(" AUTOMATIC BILL ");
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
  lcd.print("Please add item:");
}

void loop() {
  char input[13];
  int count = 0;

  while (Serial.available() && count < 12) {
    input[count] = Serial.read();
    count++;
    delay(5);
  }
  input[count] = '\0';

  int a = digitalRead(A4);
  bool itemFound = false;

  for (int i = 0; i < numItems; i++) {
    if ((strncmp(input, items[i].RFID, 12) == 0) && (a == 1)) {
      lcd.begin(16,2);
      lcd.print("Item: ");
      lcd.print(items[i].name); // Display item name
      lcd.setCursor(0,1);
      lcd.print("Price: ");
      lcd.print(items[i].price);
      items[i].quantity++;
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(2000);
      total += items[i].price;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price : ");
      lcd.setCursor(0,1);
      lcd.print(total);
      count_prod++;
      digitalWrite(greenLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      itemFound = true;
      break;
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

  
}