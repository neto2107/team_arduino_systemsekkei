
void setupSonic(){
  pinMode(TRIG, OUTPUT);         //trigを出力ポートに設定
  pinMode(ECHO, INPUT);          //echoを入力ポートに設定
}

//距離を計測
int distance() {
  unsigned long interval; //Echo のパルス幅（μs）
  int dst; //距離（cm）
  digitalWrite(TRIG, HIGH); //10μs のパルスを超音波センサの Trig ピンに出力
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //Echo が HIGH である時間（μs）を計測
  //5767μs(100cm)、超音波が反射してこなければタイムアウトしてエラー値 0 を返す
  interval = pulseIn(ECHO, HIGH, 5767);
  dst = (0.61 * 25 + 331.5) * interval / 10000 / 2; //時間（μs）を距離（cm）に変換
  //if (dst == 0) {dst = 100;} //エラー値 0（超音波が反射してこない）は 100cm を検出したことにする
  delay(60); //trig が HIGH になる間隔を 60ms 以上あける（超音波センサの仕様）
  return dst;
}
