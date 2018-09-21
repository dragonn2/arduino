#include <Wire.h>
#include <TEA5767Radio.h>
#include <EEPROM.h>
// Initialize the Radio
TEA5767Radio radio = TEA5767Radio();
int button1 = 4;
int button2 = 5;
float frequancy = 97.65f; //default frequancy
float frequancyTmp = frequancy;

void setup() {
  // Start the I2C interface
  Wire.begin();
  Serial.begin(9600);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  //restore last saved frequency from EEPROM
  restoreStation();
}
void loop() {
  setRadio();
  delay(250);
}

void setRadio() {
  int btn1State = digitalRead(button1);
  int btn2State = digitalRead(button2);

  if  ((btn1State == 1) && (btn2State == 1))
  {
    Serial.print("Save to EEPROM: ");
    Serial.println(frequancy);
    EEPROM.put(2,frequancy);
    for (int i = 0; i <= 4; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  } else if (btn1State == 1) {
    frequancyTmp = frequancyTmp - 0.05f;
  } else if (btn2State == 1) {
    frequancyTmp = frequancyTmp + 0.05f;
  }
  if (frequancy != frequancyTmp) {
    frequancy = frequancyTmp;
    Serial.println(frequancy);
    radio.setFrequency(frequancy);
  }
}

void restoreStation() {
  float f = 0.00f;
  EEPROM.get(2,f);
  Serial.println(f);
  if (isnan(f)) {
    Serial.print("Save to EEPROM: ");
    Serial.println(frequancy);
    EEPROM.put(2,frequancy);
  } else {
    Serial.print("Restored from EEPROM: ");
    Serial.println(f);
    frequancy = f;
    frequancyTmp = f;
  }
  // HERE: adjust the frequency
  radio.setFrequency(frequancy);
}


