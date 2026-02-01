int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
  int s6 = 0;
  int s7 = 0;
  int s8 = 0;
  int s9 = 0;
  
#define led 13
int spd = 75;
const int lmf = A5;
const int lmb = A4;

const int rmb = A3;
const int rmf = A2;

#define lpwm 10
#define rpwm 11




void setup() {
  for(int i = 2; i <= 9; i++)
  pinMode(i, INPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);

  pinMode(lpwm, OUTPUT);
  pinMode(rpwm, OUTPUT);

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

  if (s4 == 0 || s5 == 0)
  {
    forward () ;
    digitalWrite (led, LOW);
  }
   if(s6 == 0 || s7 == 0 || s8 == 0)
   {
    right () ;
    digitalWrite (led, LOW);
   }
   if(s1 == 0 || s2 == 0 || s3 == 0)
   {
    left ();
    digitalWrite (led, LOW);
   }
   if ((s1 == 0 && s8 == 0 && s4 == 0)|| (s1 == 0 && s5 == 0 && s8 == 0) || (s1 == 0 && s5 == 0 && s4 == 0 && s8 == 0) ) {
   analogWrite (led, 255);
   delay(5);
   }

   if (s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0 && s5 == 0 && s6 == 0 && s7 == 0 && s8 == 0 && s9 == 0)
  Stop () ;

}

void forward() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  delay(10);
}

void right() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  delay(10);
}
void left() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
  delay(10);
}

void Stop() {
  analogWrite(lpwm, spd);
  analogWrite(rpwm, spd);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(led, HIGH);
  
}