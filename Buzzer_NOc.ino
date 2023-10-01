#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int resetPin = A0; // Define reset button pin
int buttons[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // Array to store button pins
int pressedSequence[9]; // Array to store the sequence of pressed buttons (up to 9 presses)
int sequenceIndex = 0; // Index for the pressedSequence array
bool flag = false;
void setup()
{
  for (int i = 0; i < 8; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  pinMode(resetPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("8-Ch Buzzer");
  lcd.setCursor(5, 1);
  lcd.print("System");
  delay(1000);
}

void loop()
{
  int pressedButton = checkButtons(); // Check which button was pressed
  if (digitalRead(resetPin) == HIGH) {
     // Clear the LCD when reset button is pressed
    sequenceIndex = 0; // Reset the sequence index
    if (!flag){
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Reset!!");
      lcd.setCursor(1, 1);
      lcd.print("Release Button");
    }
    flag = true;
  } else {
    if(flag){
      lcd.clear();
       lcd.setCursor(2, 0);
       lcd.print("8-Ch Buzzer");
       lcd.setCursor(5, 1);
       lcd.print("System");
       flag = false;
    }
    int pressedButton = checkButtons(); // Check which button was pressed
    if (pressedButton != -1 && sequenceIndex < 9) {
      pressedSequence[sequenceIndex] = pressedButton; // Store the pressed button in the sequence array
      sequenceIndex++; // Increment the sequence index
      displaySequence(); // Display the sequence on the LCD

    }
  }
}

int checkButtons() {
  int pressedButton = -1;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(buttons[i]) == HIGH) {
      //delay(50); // Debounce delay
      if (digitalRead(buttons[i]) == HIGH) {
        int buttonNumber = i + 1;
        if (!isButtonInSequence(buttonNumber)) {
          pressedButton = buttonNumber;
        }
      }
    }
  }
  return pressedButton; // Return button number (1 to 12) if it's not in the sequence, otherwise return -1
}

bool isButtonInSequence(int buttonNumber) {
  for (int i = 0; i < sequenceIndex; i++) {
    if (pressedSequence[i] == buttonNumber) {
      return true; // Button is already in the sequence
    }
  }
  return false; // Button is not in the sequence
}

void displaySequence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sequence:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < sequenceIndex; i++) {
    lcd.print(pressedSequence[i]);
    lcd.print(" ");
  }
}
