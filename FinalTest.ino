#include <Wire.h>
#include <SoftwareSerial.h>
#include <MQUnifiedsensor.h>
#include <LiquidCrystal_I2C.h>

#define placa "Arduino NANO"
#define Voltage_Resolution 5
#define pin A1
#define type "MQ-135"
#define ADC_Bit_Resolution 10

const float customRo = 59.0;
int Buzzer = 5;
int Call = 0;
int OzoneSensor = A2;
String category = "Loading...";
unsigned long previousMillis = 0;
const unsigned long interval = 3000;

LiquidCrystal_I2C lcd(0x3F, 20, 4);
SoftwareSerial sim800lSerial(2, 3);

MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  sim800lSerial.begin(9600);
  sendATCommand("AT");  // Send AT command to check communication
  delay(1000);
  sendATCommand("AT+CMEE=2");  // Enable error reporting in verbose mode
  delay(1000);

  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

  MQ135.setRegressionMethod(1);
  MQ135.init();
  delay(50);

  // Set the custom Ro value
  MQ135.setR0(customRo);
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print("SYSTEM");
  lcd.setCursor(2, 2);
  lcd.print("INITIALIZATION");
  delay(3000);
}
void loop() {
  MQ135.update();  // Update data, the Arduino will read the voltage from the analog pin

  MQ135.setA(605.18);
  MQ135.setB(-3.937);             // Configure the equation to calculate CO concentration value
  float CO = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(77.255);
  MQ135.setB(-3.18);                   // Configure the equation to calculate Alcohol concentration value
  float Alcohol = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(110.47);
  MQ135.setB(-2.862);              // Configure the equation to calculate CO2 concentration value
  float CO2 = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(44.947);
  MQ135.setB(-3.445);                 // Configure the equation to calculate Toluen concentration value
  float Toluen = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(102.2);
  MQ135.setB(-2.473);              // Configure the equation to calculate NH4 concentration value
  float NH4 = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(34.668);
  MQ135.setB(-3.369);                 // Configure the equation to calculate Aceton concentration value
  float Aceton = MQ135.readSensor();  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  int OzoneValue = analogRead(OzoneSensor);

  if (OzoneValue > 0 && OzoneValue < 350) {
    category = " Normal";
    digitalWrite(Buzzer, LOW);
  } else if (OzoneValue > 350 && OzoneValue < 700) {
    category = " Medium";
    digitalWrite(Buzzer, LOW);
  } else if (OzoneValue > 700) {
    category = "High";
    digitalWrite(Buzzer, LOW);
  }
  if (CO < 500) {
    digitalWrite(Buzzer, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO ");
    lcd.setCursor(10, 2);
    lcd.print(CO, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Alcohol ");
    lcd.setCursor(15, 2);
    lcd.print(Alcohol, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO2 ");
    lcd.setCursor(11, 2);
    lcd.print(CO2 + 400, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Toluen ");
    lcd.setCursor(14, 2);
    lcd.print(Toluen, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: NH4 ");
    lcd.setCursor(11, 2);
    lcd.print(NH4, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Aceton ");
    lcd.setCursor(14, 2);
    lcd.print(Aceton, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: NORMAL");
    delay(1500);
  } else if (CO > 500 && CO < 10000) {
    digitalWrite(Buzzer, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO ");
    lcd.setCursor(10, 2);
    lcd.print(CO, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Alcohol ");
    lcd.setCursor(15, 2);
    lcd.print(Alcohol, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO2 ");
    lcd.setCursor(11, 2);
    lcd.print(CO2 + 400, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Toluen ");
    lcd.setCursor(14, 2);
    lcd.print(Toluen, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: NH4 ");
    lcd.setCursor(11, 2);
    lcd.print(NH4, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Aceton ");
    lcd.setCursor(14, 2);
    lcd.print(Aceton, 2);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: MEDIUM");
    delay(1500);
  } else if (CO > 10000) {
    digitalWrite(Buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO ");
    lcd.setCursor(10, 2);
    lcd.print(CO, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3 ");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Alcohol ");
    lcd.setCursor(15, 2);
    lcd.print(Alcohol, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: CO2 ");
    lcd.setCursor(11, 2);
    lcd.print(CO2 + 400, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Toluen ");
    lcd.setCursor(14, 2);
    lcd.print(Toluen, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: NH4 ");
    lcd.setCursor(11, 2);
    lcd.print(NH4, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AIR QUALITY ANALYZER");
    lcd.setCursor(0, 1);
    lcd.print("MQ131: O3");
    lcd.setCursor(10, 1);
    lcd.print(category);
    lcd.setCursor(0, 2);
    lcd.print("MQ135: Aceton ");
    lcd.setCursor(14, 2);
    lcd.print(Aceton, 1);
    lcd.setCursor(0, 3);
    lcd.print("STATUS: CRITICAL");
    delay(1500);
    
    BuzzerStatus();
    
  }
}

void BuzzerStatus() {
  if (digitalRead(Buzzer) == HIGH && Call == 0) {
    callNumber("+254748613509");

    delay(5000);  

    // Hang up the call
    hangUpCall();
    Call = 1;
  }else {
    Call = 0;
  }
}
void sendATCommand(String command) {
  sim800lSerial.println(command);
  delay(500);
  while (sim800lSerial.available()) {
    Serial.write(sim800lSerial.read());
  }
}

void callNumber(String phoneNumber) {
  String callCommand = "ATD" + phoneNumber + ";";
  sendATCommand(callCommand);
}

void hangUpCall() {
  sendATCommand("ATH0");
}