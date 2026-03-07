const int lmf = A5;
const int lmb = A4;
const int rmb = A3;
const int rmf = A2;

#define lpwm 10
#define rpwm 11

// Speed
int baseSpeed = 85;
int maxSpeed = 200;
int minSpeed = 45;

// PID Constants
float Kp = 8.0;
float Ki = 0.0003;
float Kd = 28.0;

// PID Variables
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;
float correction = 0;

// Sensor weights
float sensorWeight[8] = {-4, -3, -2, -1, 1, 2, 3, 4};

// Gap handling
int lastDir = 1;
bool gapMode = false;
unsigned long gapStart = 0;
const int gapDelay = 110;


void setup() 
{
  for(int i = 2; i <= 9; i++) pinMode(i, INPUT);

  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(lpwm, OUTPUT);
  pinMode(rpwm, OUTPUT);

  Serial.begin(9600);
}

void loop() 
{

  int s[8];
  for(int i = 0; i < 8; i++)
    s[i] = digitalRead(i + 2);

  int activeSensors = 0;
  float weightedSum = 0;

  for(int i = 0; i < 8; i++) 
  {
    if(s[i] == 0) 
    {   // black line
      weightedSum += sensorWeight[i];
      activeSensors++;
    }
  }

  // Lost Line Handling
  if(activeSensors == 0) 
  {  
    if(!gapMode) 
    {
      gapMode = true;
      gapStart = millis();
    }

    if(millis() - gapStart < gapDelay) 
    {
      driveMotors(baseSpeed, baseSpeed);
    } 
    else 
    {
      if(lastDir == -1)
        driveMotors(-minSpeed, minSpeed);
      else
        driveMotors(minSpeed, -minSpeed);
    }
    return;
  }

  gapMode = false;

  error = weightedSum / activeSensors;

  // Small deadzone to reduce micro oscillation
  if(abs(error) < 0.25) error = 0;

  if(error < -0.5) lastDir = -1;
  else if(error > 0.5) lastDir = 1;

  // ===== PID CONTROL =====
  integral += error;
  integral = constrain(integral, -30, 30);  // reduced windup

  derivative = error - lastError;

  correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

  lastError = error;

  int dynamicSpeed;

  if(abs(error) < 0.8)
    dynamicSpeed = baseSpeed;          // straight
  else if(abs(error) < 2)
    dynamicSpeed = baseSpeed - 25;     // medium turn
  else
    dynamicSpeed = baseSpeed - 50;     // sharp turn

  dynamicSpeed = constrain(dynamicSpeed, minSpeed, baseSpeed);

  int leftSpeed  = dynamicSpeed + correction;
  int rightSpeed = dynamicSpeed - correction;

  //Prevent reverse during normal tracking
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  driveMotors(leftSpeed, rightSpeed);

  // Debug
  Serial.print("Err: "); Serial.print(error);
  Serial.print(" | L: "); Serial.print(leftSpeed);
  Serial.print(" | R: "); Serial.println(rightSpeed);
}

void driveMotors(int leftSpeed, int rightSpeed) 
{

  // LEFT MOTOR
  if(leftSpeed >= 0) 
  {
    analogWrite(lpwm, leftSpeed);
    digitalWrite(lmf, HIGH);
    digitalWrite(lmb, LOW);
  } 
  else 
  {
    analogWrite(lpwm, -leftSpeed);
    digitalWrite(lmf, LOW);
    digitalWrite(lmb, HIGH);
  }

  // RIGHT MOTOR
  if(rightSpeed >= 0) 
  {
    analogWrite(rpwm, rightSpeed);
    digitalWrite(rmf, HIGH);
    digitalWrite(rmb, LOW);
  } 
  else 
  {
    analogWrite(rpwm, -rightSpeed);
    digitalWrite(rmf, LOW);
    digitalWrite(rmb, HIGH);
  }
}


void Stop() 
{
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
}
