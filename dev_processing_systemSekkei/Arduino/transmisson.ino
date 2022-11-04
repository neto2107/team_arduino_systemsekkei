byte inByte;
unsigned long tusinTimePrev = 0;
void recvTusin(){
    if(tusinState(50)){//50msごとに通信
        if(Serial.available()>0){
            inByte = Serial.read();
            if(inByte == 0xff){
                Serial.write('H'); //文字0x320をバイナリデータとして送信
                sendInt(10);
                tusinTimePrev = timeNow_G;
            }
        }
    }

    if(timeNow_G - tusinTimePrev > 1000){
        Serial.write('H');
        sendInt(10);
    }
    
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
