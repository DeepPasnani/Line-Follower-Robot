#define IN1 5
#define IN2 4
#define IN3 3
#define IN4 2
#define ENA 10
#define ENB 11
#define LED 13

#define SENSOR_COUNT 6
int sensorPins[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5};
int sensorValues[SENSOR_COUNT];
int threshold[SENSOR_COUNT] = {500, 500, 500, 500, 500, 500};

float Kp = 40.0;
float Kd = 700.0;
float Ki = 0.0;
int baseSpeed = 180;
int maxSpeed = 250;
int turnSpeed = 140;

float center = 3.5;
int weights[SENSOR_COUNT] = {1, 2, 3, 4, 5, 6};

float lastError = 0;
float integral = 0;
float integralMax = 100;

unsigned long startTime;
bool started = false;
unsigned long intersectionTime = 0;
bool inIntersection = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  digitalWrite(LED, HIGH);
  startTime = millis();
}

void loop() {
  if (!started) {
    if (millis() - startTime >= 2000) {
      started = true;
      digitalWrite(LED, LOW);
    }
    return;
  }
  
  if (inIntersection) {
    if (millis() - intersectionTime >= 200) {
      inIntersection = false;
    } else {
      setMotors(baseSpeed, baseSpeed);
      return;
    }
  }
  
  readSensors();
  
  int onLine = 0;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorValues[i] == 1) onLine++;
  }
  
  if (onLine == 0) {
    integral = 0;
    handleLineLost();
    return;
  }
  
  if (onLine >= 4) {
    integral = 0;
    handleIntersection();
    return;
  }
  
  float position = calculatePosition();
  float error = position - center;
  
  integral += error;
  integral = constrain(integral, -integralMax, integralMax);
  float derivative = error - lastError;
  
  float correction = Kp * error + Ki * integral + Kd * derivative;
  
  lastError = error;
  
  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;
  
  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);
  
  setMotors(leftSpeed, rightSpeed);
}

void readSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int raw = analogRead(sensorPins[i]);
    sensorValues[i] = (raw > threshold[i]) ? 1 : 0;
  }
}

float calculatePosition() {
  float weightedSum = 0;
  int totalActive = 0;
  
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (sensorValues[i] == 1) {
      weightedSum += weights[i];
      totalActive++;
    }
  }
  
  if (totalActive == 0) {
    return center;
  }
  
  return weightedSum / totalActive;
}

void handleLineLost() {
  if (lastError > 0) {
    setMotors(turnSpeed, -turnSpeed);
  } else {
    setMotors(-turnSpeed, turnSpeed);
  }
}

void handleIntersection() {
  intersectionTime = millis();
  inIntersection = true;
}

void setMotors(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, abs(leftSpeed));
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(leftSpeed));
  }
  
  if (rightSpeed >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, abs(rightSpeed));
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(rightSpeed));
  }
}
