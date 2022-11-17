
void setupSonic(){
  pinMode(TRIG, OUTPUT);         //trigを出力ポートに設定
  pinMode(ECHO, INPUT);          //echoを入力ポートに設定
}

//距離を計測
int distance() {
  unsigned long interval;        //Echoのパルス幅(μs)
  int dist;                      //距離(cm)
  
  digitalWrite(TRIG, HIGH);      //10μsのパルスを超音波センサのTrigピンに出力
  delayMicroseconds(10);
  digitalWrite(ECHO, LOW);

  interval = pulseIn(ECHO, HIGH, 23071);               //Echo信号がHIGHである時間(μs)を計測
  dist = (0.61 * 25 + 331.5) * interval / 10000 / 2;   //距離(cm)に変換

  delay(60);                     //trigがHIGHになる間隔を60ms以上空ける（超音波センサの仕様）

  return dist;
}
