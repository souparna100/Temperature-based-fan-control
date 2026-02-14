#include <DHT.h>

// -------- SENSOR CONFIG --------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------- MOTOR DRIVER PINS --------
#define ENA 9     // PWM speed control
#define IN1 7
#define IN2 8

// -------- TEMPERATURE SETTINGS --------
float thresholdTemp = 30.0;   // °C — motor turns ON above this
float maxTemp = 35.0;         // °C — full speed at this temp

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor OFF initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  Serial.println("Temperature Regulation System Started");
}

void loop() {
  delay(2000); // DHT11 needs ~2s between reads

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Sensor read failed!");
    return;
  }

  // -------- DISPLAY --------
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  // -------- CONTROL LOGIC --------
  if (temp <= thresholdTemp) {
    // Motor OFF
    stopMotor();
    Serial.println("Motor OFF — Temp below threshold");
  } 
  else {
    // Map temperature to PWM speed
    int speedPWM = map(temp, thresholdTemp, maxTemp, 120, 255);
    speedPWM = constrain(speedPWM, 120, 255);

    runMotor(speedPWM);

    Serial.print("Motor ON — Speed PWM: ");
    Serial.println(speedPWM);
  }

  Serial.println("----------------------");
}

// -------- MOTOR FUNCTIONS --------

void runMotor(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedVal);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}
