
int sof_f = 0; //SOFを発見したかどうかのフラグ
int l;//受信バッファ内のデータ数
void serialEvent(Serial p) {
  l = p.available();//受信バッファないのデータ数を取得
  if (p == port_G&&l>=12) {
    recvManager(p, robot1);
  }
}

int recvManager(Serial p, Robot robo) {
  while (l>0) {//受信バッファないにデータがある時
    if (sof_f == 0) { // SoF を発見していない場合
      int sof = p.read();
      if (sof == 'C') { // SoF:Start of Frame の検査
        sof_f = 1; // SoF の発見を記録('C'(カラーセンサーの値を受け取り):1
      }
      if (sof == 'G') {
        sof_f = 2;//コンパスセンサーの値を受け取り
      }
      if (sof == 'S') {
        sof_f = 3;//超音波センサーの値受け取り
      }
      if (sof == 'A') {
        sof_f = 4;//超音波センサーの値受け取り
      }
      if (sof == 'N') {
        println("<--N");
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f = 0;
      }
      l--;//受信バッファ数を修正
    }

    //カラーセンサーの値受け取り
    if (sof_f == 1) {//データの受信を検知したら
      if (l >= 3) { //受信バッファに到着しているデータ数が2以上なら
        //受信したデータを格納
        color c = recvRGB(p);
        robo.setColorSensorValue(c); //ロボットにカラーセンサーの値をセット

        l-=3; //受信した分を減らす
        println("<-C"); //データ受信タイミング
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      } else {
        break;
      }
    }
    //コンパスセンサーの値受け取り
    if (sof_f == 2) {
      if (l>=2) {

        robo.set_degree(recvCompass(p)); //角度を取得する

        l-=2; //受信した分を減らす
        println("<-G"); //データ受信タイミング
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      } else {
        sof_f = 0;
      }
    }
    //超音波センサーの値受け取り
    if (sof_f == 3) {
      if (l>=2) {

        robo.setUltrasonicSensingDistance(recvSonic(p) * 10);

        l-=2; //受信した分を減らす
        println("<-G"); //データ受信タイミング
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      } else {
        sof_f = 0;
      }
    }
    //超音波センサーの値受け取り
    if (sof_f == 4) {
      if (l>=9) {
        //受信したデータを格納
        color c = recvRGB(p);
        robo.setColorSensorValue(c); //ロボットにカラーセンサーの値をセット
        robo.set_degree(recvCompass(p)); //角度を取得する
        robo.setUltrasonicSensingDistance(recvSonic(p) * 10);
        robo.setAccel(recvAccel(p)); //加速度の格納と表示
        robo.setSpeed(recvSpeed(p));
        println(robo.getAccel());
        l-=11; //受信した分を減らす
        println("<-A"); //データ受信タイミング
        port_G.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      } else {
        sof_f = 0;
      }
    }
  }
  return 0;
}
//コンパスの情報を受信するメソッド
int recvCompass(Serial p) {
  int compass = recvInt(p);
  return compass;
}

//色を受信するメソッド
color recvRGB(Serial p) {
  int r = p.read();
  int g = p.read();
  int b = p.read();
  color c = color(r, g, b);
  return c;
}

//超音波センサーを受信するメソッド
int recvSonic(Serial p) {
  return recvInt(p);
}

int recvSpeed(Serial p){
  return recvInt(p);
}

int recvAccel(Serial p){
  return recvInt(p);
}

//ArduinoからInt型の値を受信する
int recvInt(Serial p) {
  int high = p.read();//上位8ビットを読み込む
  int low = p.read();//下位8ビットを読み込む
  int val = (high << 8) + low;//上位8ビットシフトして足すことで、1つの変数に戻す
  if (32767 < val)//負の値かどうかが保存されないので、該当するときは場合分け
    val -=65536;//65536 = 2^16

  return val;//得られたInt型の値を返す
}
