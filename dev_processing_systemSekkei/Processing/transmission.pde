
int sof_f = 0; //SOFを発見したかどうかのフラグ
void serialEvent(Serial p){
    int l = p.available();//受信バッファないのデータ数を取得
  while(l>0){//受信バッファないにデータがある時
    if (sof_f == 0) { // SoF を発見していない場合
      int sof = p.read();
      if (sof == 'H') { // SoF:Start of Frame の検査
        sof_f = 1; // SoF の発見を記録('H'(照度センサーの値を受け取り):1
      }
      l--;//受信バッファ数を修正
    }
  
  if(sof_f == 1){//データの受信を検知したら
    if(l >= 2){ //受信バッファに到着しているデータ数が2以上なら
        //受信したデータを格納
        int recv = recvInt(p);
        
      
        l-=2; //受信した分を減らす
        println("<-H"); //データ受信タイミング
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      }else{
        break;
      }
  }
  }
}

//ArduinoからInt型の値を受信する
int recvInt(Serial p){
  int high = p.read();//上位8ビットを読み込む
  int low = p.read();//下位8ビットを読み込む
  int val = (high << 8) + low;//上位8ビットシフトして足すことで、1つの変数に戻す
  if(32767 < val)//負の値かどうかが保存されないので、該当するときは場合分け
    val -=65536;//65536 = 2^16
    
  return val;//得られたInt型の値を返す
}
