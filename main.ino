#include <GyverNTC.h>

// Определение пинов и констант
#define TdsSensorPin A0
#define pHSensorPin A1
#define VREF 5.0
#define ADC_RESOLUTION 1024.0  
GyverNTC therm(1, 10000, 3950);  // Температурный датчик

float tdsFactor = 0.5;
float temperature = 25;
const int trigPin = 11;  // Триггер для ультразвукового датчика
const int echoPin = 12;  // Эхо для ультразвукового датчика

unsigned long previousMillis = 0;
const long interval = 10000; // Интервал 10 секунд

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

// Функция для измерения TDS
float getTDSValue() {
  int sensorValue = analogRead(TdsSensorPin);
  float voltage = sensorValue * (VREF / ADC_RESOLUTION);
  float tdsValue = (voltage / VREF) * 1000;
  float ppmValue = tdsValue / (1.0 + 0.02 * (temperature - 25));
  return ppmValue;
}

// Функция для измерения pH
float getpH() {
  int sensorValue = analogRead(pHSensorPin);  // Чтение значения с аналогового пина pH сенсора
  float voltage = sensorValue * (VREF / ADC_RESOLUTION);  // Преобразование значения в напряжение
  float pHValue = 7 + ((2.5 - voltage) / 0.18);  // Формула для расчета pH
  return pHValue;
}

// Функция для измерения температуры
float getTemperature() {
  return therm.getTempAverage();
}

// Функция для измерения расстояния
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  unsigned long currentMillis = millis();

  // Проверяем, прошли ли 10 секунд
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Получаем данные с сенсоров
    float tdsValue = getTDSValue();
    float currentTemperature = getTemperature();
    int distance = getDistance();
    float pHValue = getpH();  // Получаем значение pH

    // Создаем массив для хранения данных
    float data[4];
    data[0] = tdsValue;
    data[1] = currentTemperature;
    data[2] = distance;
    data[3] = pHValue;

    // Печатаем элементы массива в одну строку
    Serial.print(data[0]);
    Serial.print(",");
    Serial.print(data[1]);
    Serial.print(",");
    Serial.print(data[2]);
    Serial.print(",");
    Serial.println(data[3]);  // Выводим pH значение
  }
}
