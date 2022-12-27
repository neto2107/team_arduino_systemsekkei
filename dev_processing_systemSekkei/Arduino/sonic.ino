
void setupSonic(){
  pinMode(TRIG, OUTPUT);         //trigを出力ポートに設定
  pinMode(ECHO, INPUT);          //echoを入力ポートに設定
}

unsigned long echo_time_prev=0;
int echo_pre_dist=0;
//距離を計測
int distance() {
  if(timeNow_G - echo_time_prev < 60){
    return echo_pre_dist;
  }
  unsigned long interval; //Echo のパルス幅（μs）
  
  int dst; //距離（cm）
  digitalWrite(TRIG, HIGH); //10μs のパルスを超音波センサの Trig ピンに出力
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //Echo が HIGH である時間（μs）を計測
  //5767μs(100cm)、超音波が反射してこなければタイムアウトしてエラー値 0 を返す
  interval = pulseIn(ECHO, HIGH, 5601);
  dst = (0.61 * 25 + 331.5) * interval / 10000 / 2; //時間（μs）を距離（cm）に変換
  //if (dst == 0) {dst = 100;} //エラー値 0（超音波が反射してこない）は 100cm を検出したことにする
  echo_time_prev = timeNow_G;
  echo_pre_dist = dst;
  return dst;
}
