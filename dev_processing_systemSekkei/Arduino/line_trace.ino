
void linetrace_init(){
  now_speed = speed100;
  speed0 = 100;
}
//引数
//回転方向0→反時計回り
//回転方向1→時計回り
float linetrace_P(bool right_rotate)
{
  static float lightMin = (r_min + g_min + b_min) /3.0; // 各自で設定
  static float lightMax = (r_max + g_max + b_max) /3.0; // 各自で設定 （わざとエラーが出るようにしてある）
  static float Kp = 0; // パラメーター
  float lightNow;//現在のカラーセンサーのグレースケール値
  float speedDiff;//補正用の速度の偏差

  //カラーセンサーの値をグレースケールに変換
  lightNow = (r_G + g_G + b_G ) / 3.0;
  //現在のグレースケール-明るさの平均値を計算することでKp(どれだけ回転させるかを決めるパラメータ)を算出
  //平均より小さい→暗い（ライン上にある)→負の値
  //平均より大きい→明るい(ライン上にない。)→正の値
  Kp = lightNow - ((lightMin + lightMax) / 2.0);
  //回転方向によって(motor_mode_Gによって)分岐させる
  //Serial.println("speedDiff:%f",speedDiff);
  if(right_rotate == false){
    speedDiff = Kp/70 *(float)speed0;//反時計回り
  }else{
    speedDiff = -Kp/70 *(float)speed0;//時計回り(speedDiffの正負を判定させる。)
  }
  return speedDiff;
}