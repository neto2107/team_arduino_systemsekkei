byte inByte;
unsigned long timePrevList[5] = {0, 0, 0, 0, 0};


//通信を受信する関数
void recvTusin() {
//  if (timeNow_G - timePrevList[0] > 500) { //50msごとに通信
//    if (Serial.available() > 0) {
//      inByte = Serial.read();
//      if (inByte == 0xff) {
//        Serial.write('C'); //文字をバイナリデータとして送信
//        sendRGB();
//        timePrevList[0] = timeNow_G;
//        timePrevList[1] = timeNow_G;
//      }
//    }
//  }
//  if (timeNow_G - timePrevList[2] > 500) { //50msごとに通信
//    if (Serial.available() > 0) {
//      inByte = Serial.read();
//      if (inByte == 0xff) {
//        Serial.write('G'); //文字をバイナリデータとして送信
//        sendCompass();
//        timePrevList[2] = timeNow_G;
//        timePrevList[1] = timeNow_G;
//      }
//    }
//  }
//  if (timeNow_G - timePrevList[3] > 500) { //50msごとに通信
//    if (Serial.available() > 0) {
//      inByte = Serial.read();
//      if (inByte == 0xff) {
//        Serial.write('S'); //文字をバイナリデータとして送信
//        sendSonic();
//        timePrevList[3] = timeNow_G;
//        timePrevList[1] = timeNow_G;
//      }
//    }
//  }
  if(timeNow_G - timePrevList[4] > 100){
        if (Serial.available() > 0) {
      inByte = Serial.read();
      if (inByte == 0xff) {
        Serial.write('A'); //文字をバイナリデータとして送信
        sendRGB();
        sendCompass();
        sendSonic();
        sendAccel();
        timePrevList[4] = timeNow_G;
        timePrevList[1] = timeNow_G;
      }
    }
  }
  if (timeNow_G - timePrevList[1] > 1000) {
    Serial.write('N');
    timePrevList[1] = timeNow_G;
  }

}

void sendRGB() {
  int r = r_G, g = g_G, b = b_G;
  Serial.write(r);
  Serial.write(g);
  Serial.write(b);
}

void sendCompass() {
  sendInt(heading_G);
}

void sendSonic(){
  sendInt(dist_G);
}

void sendAccel(){
  sendInt(ax);
}

//Int型の数値をsereal.writeで送信する
void sendInt(int n)
{
  int high = n >> 8; //上位8ビットをhighに代入
  int low = n & 255; //下位8ビットをlowに代入(下位8ビットがすべて1の値とAnd演算することで、上位8ビットがすべて0になる)
  Serial.write(high);
  Serial.write(low);
}


int tusinState( unsigned long period )
{
  static int flagStart = 0; // 0:待ち状態，1:現在計測中
  static unsigned long startTime = 0;

  if ( flagStart == 0 ) {
    startTime = timeNow_G; // 計測を開始したtimeNow_Gの値を覚えておく
    flagStart = 1; // 現在計測中にしておく
  }

  if ( timeNow_G - startTime > period ) { // 計測開始からの経過時間が指定時間を越えた
    flagStart = 0; // 待ち状態に戻しておく
    startTime = 0; // なくても良いが，形式的に初期化
    return 1;
  }
  else
    return 0;
}
