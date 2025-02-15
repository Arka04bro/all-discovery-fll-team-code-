#include <GyverNTC.h>

#define tdsPin A0
#define phPin A1
#define vRef 5.0
#define adcRes 1024.0  
GyverNTC tempSensor(1, 10000, 3950);

float tdsFactor = 0.5;
float temp = 25;
const int trig = 11;
const int echo = 12;

const int joy1X = A2;
const int joy1Y = A3;
const int joy2X = A4;
const int joy2Y = A5;

unsigned long prevMillis = 0;
const long joyInterval = 50;

unsigned long prevSensMillis = 0;
const long sensInterval = 10000;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

float getTds() {
  int val = analogRead(tdsPin);
  float volt = val * (vRef / adcRes);
  float tds = (volt / vRef) * 1000;
  return tds / (1.0 + 0.02 * (temp - 25));
}

float getPh() {
  int val = analogRead(phPin);
  float volt = val * (vRef / adcRes);
  return 7 + ((2.5 - volt) / 0.18);
}

float getTemp() {
  return tempSensor.getTempAverage();
}

int getDist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long dur = pulseIn(echo, HIGH);
  return dur * 0.034 / 2;
}

void loop() {
  unsigned long currMillis = millis();

  if (currMillis - prevMillis >= joyInterval) {
    prevMillis = currMillis;

    int j1x = analogRead(joy1X);
    int j1y = analogRead(joy1Y);
    int j2x = analogRead(joy2X);
    int j2y = analogRead(joy2Y);

    Serial.print("J,");
    Serial.print(j1x);
    Serial.print(",");
    Serial.print(j1y);
    Serial.print(",");
    Serial.print(j2x);
    Serial.print(",");
    Serial.println(j2y);
  }

  if (currMillis - prevSensMillis >= sensInterval) {
    prevSensMillis = currMillis;

    float tds = getTds();
    float temp = getTemp();
    int dist = getDist();
    float ph = getPh();

    Serial.print("S,");
    Serial.print(tds);
    Serial.print(",");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(dist);
    Serial.print(",");
    Serial.println(ph);
  }
}
