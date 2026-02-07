int s1, s2, s3, s4, s5, s6, s7, s8, s9;
#define led 13
int spd = 90;
const int lmf = A5;
const int lmb = A4;
const int rmb = A3;
const int rmf = A2;
#define lpwm 10
#define rpwm 11
int lastTurn = 0;  
// -1 = left, +1 = right
bool lineSearching = false;
unsigned long lostLineTime = 0;

void setup() {
  for (int i = 2; i <= 9; i++)
    pinMode(i, INPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(lpwm, OUTPUT);
  pinMode(rpwm, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  s1 = digitalRead(2);
  s2 = digitalRead(3);
  s3 = digitalRead(4);
  s4 = digitalRead(5);
  s5 = digitalRead(6);
  s6 = digitalRead(7);
  s7 = digitalRead(8);
  s8 = digitalRead(9);
  s9 = digitalRead(12);
  
  // ---------- NORMAL LINE FOLLOW ----------
  if (s4 == 0 || s5 == 0) {
    forward();
    digitalWrite(led, LOW);
    lineSearching = false;  // Reset search flag
  }
  else if (s6 == 0 || s7 == 0 || s8 == 0) {
    right();
    lastTurn = 1;
    digitalWrite(led, LOW);
    lineSearching = false;  // Reset search flag
  }
  else if (s1 == 0 || s2 == 0 || s3 == 0) {
    left();
    lastTurn = -1;
    digitalWrite(led, LOW);
    lineSearching = false;  // Reset search flag
  }
  // ---------- LOST LINE (all sensors show white) ----------
  else if (s1==1 && s2==1 && s3==1 && s4==1 && s5==1 && s6==1 && s7==1 && s8==1) {
    if (!lineSearching) {
      // First time losing the line - move forward and mark the time
      forward();
      lostLineTime = millis();
      lineSearching = true;
    }
    else {
        if (millis() - lostLineTime >= 250) {
        if (lastTurn == -1) {
          left();     // search left
        } else {
          right();    // search right
        }
      }
      else {
        forward();
      }
    }
  }
}

// ---------------- MOTOR FUNCTIONS ----------------
void forward() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
}

void right() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
}

void left() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
}

void Stop() {
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(led, HIGH);
}
