volatile unsigned long high_time = 0;
volatile unsigned long low_time = 0;
volatile int sonicTrig = 1;

void setupSonic() {
  PCICR |= B00000100;
  PCMSK2 |= B00010000;
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

ISR(PCINT2_vect) {
  unsigned long time = micros();
  if (digitalRead(ECHO) == 1) {
    high_time = time;
  } else {
    low_time = time;
    sonicTrig = 1;
  }
}

unsigned long echo_time_prev = 0;
int echo_pre_dist = 0;
//距離を計測
int distance() {
  float dist = 0;
  if (timeNow_G - echo_time_prev < 20) {
    return echo_pre_dist;
  }

  if (sonicTrig == 1) {
    digitalWrite(TRIG, HIGH);
    sonicTrig = 0;
  }
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float tempDistance = (low_time - high_time) * 0.0173375;
  if (tempDistance < 100.0) {
    dist = tempDistance;
  }else{
    dist = 0;
  }

  //if (dst == 0) {dst = 100;} //エラー値 0（超音波が反射してこない）は 100cm を検出したことにする
  echo_time_prev = timeNow_G;
  echo_pre_dist = dist;
  return dist;
}
